/******************************************************************************/
/*!
\par     Sulfur
\file    sfSAT.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/5/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSAT.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Math\sfQuaternion.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "sfGJK.hpp"

/******************************************************************************
Maxim TODO: Reduce number of contact points
*******************************************************************************/
/******************************************************************************
Maxim TODO: Check edge to edge collision
*******************************************************************************/
namespace Sulfur
{
  namespace Physics
  {
    namespace
    {
      void CheckPlanes(const Matrix4 &ma, const Matrix4 &mb,
        const ColliderGeometry *hullA, size_t index, const Geometry::Plane &plane)
      {
        ColliderGeometry::Face p = hullA->GetFace(index);

        ColliderGeometry::HalfEdge e1 = hullA->GetEdge(p.m_edge);

        std::vector<Vector3> verts;
        for (int i = 0; i < 3; ++i)
        {
          verts.push_back(hullA->GetVertex(e1.m_origin));

          e1 = hullA->GetEdge(e1.m_next);
        }

        for (auto &it : verts)
        {
          it = ma * it;
          it = mb.Inverted() * it;
        }

        Geometry::Plane pl(verts[0], verts[1], verts[2]);

        SF_ASSERT(abs(plane.GetNormal()[0] - pl.GetNormal()[0]) < 0.01, "");
        SF_ASSERT(abs(plane.GetNormal()[1] - pl.GetNormal()[1]) < 0.01, "");
        SF_ASSERT(abs(plane.GetNormal()[2] - pl.GetNormal()[2]) < 0.01, "");
        SF_ASSERT(abs(plane.GetDistance() - pl.GetDistance()) < 0.01, "");
      }
    }

    namespace SAT
    {
      Projection ProjectOnAxis(const std::vector<Vector3> &worldVerts, Vector3 &axis)
      {
        Projection proj;
        size_t numVerts = worldVerts.size();

        for (size_t vert = 0; vert < numVerts; ++vert)
        {
          Real p = Dot(axis, worldVerts[vert]);
          if (p < proj.m_min)
            proj.m_min = p;
          if (p > proj.m_max)
            proj.m_max = p;
        }

        return proj;
      }

      Real CalculatePenetration(const Projection &p1, const Projection &p2)
      {
        SF_ASSERT(p1.m_max - p2.m_min >= Real(0.0), "Penetration is negative");
        SF_ASSERT(p2.m_max - p1.m_min >= Real(0.0), "Penetration is negative");

        return std::min(p1.m_max - p2.m_min, p2.m_max - p1.m_min);
      }

      void ClipPointsToPlane(std::vector<Vector3> &verts, const Geometry::Plane &plane)
      {
        std::vector<Vector3> clippedVerts;
        if (verts.size() < 2)
        {
          verts.clear();
          return;
        }

        Vector3 v0 = verts.back();
        Vector3 v1;
        Real ds, de;
        ds = v0.Dot(plane.GetNormal()) + plane.GetDistance();

        for (size_t vi = 0; vi < verts.size(); ++vi)
        {
          v1 = verts[vi];
          de = v1.Dot(plane.GetNormal()) + plane.GetDistance();

          if (ds < 0)
          {
            if (de < 0)
              clippedVerts.push_back(v1);
            else
            {
              Real t = (ds / (ds - de));
              SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
              clippedVerts.push_back(Lerp(v0, v1, t));
            }
          }
          else
          {
            if (de < 0)
            {
              Real t = (ds / (ds - de));
              SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
              clippedVerts.push_back(Lerp(v0, v1, t));
              clippedVerts.push_back(v1);
            }
          }

          v0 = v1;
          ds = de;
        }

        verts = clippedVerts;
      }

      bool ClipLineToPlane(Vector3 &p0, Vector3 &p1, const Vector3 &plane, Real planeD)
      {
        Real dist0 = Dot(plane, p0) + planeD;
        Real dist1 = Dot(plane, p1) + planeD;

        if (dist0 > 0 && dist1 > 0)
          return false;

        if (dist0 <= 0 && dist1 > 0)
        {
          Real t = dist0 / (dist0 - dist1);
          SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
          p1 = Lerp(p0, p1, t);
        }
        else if (dist1 <= 0 && dist0 > 0)
        {
          Real t = dist0 / (dist0 - dist1);
          SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
          p0 = Lerp(p0, p1, t);
        }

        return true;
      }


      void QueryFaceDirections(const Vector3 &posA, const Vector3 &scaleA, const Quaternion &orientA,
        const ColliderGeometry *hullA, const Vector3 &posB, const Vector3 &scaleB, 
        const Quaternion &orientB, const ColliderGeometry *hullB, FaceQuery &faceQuery)
      {
        int maxIndex = -1;
        Real maxSeparation = -SF_REAL_MAX;
        Matrix4 mw;
        mw.SetTransformation(orientA, scaleA, posA);
        Matrix4 ml;
        ml.SetTransformation(orientB, scaleB, posB);
        Matrix4 aTob = ml.Inverted() * mw; 

        for (size_t index = 0; index < hullA->GetFaceCount(); ++index)
        {
          Geometry::Plane plane = hullA->GetPlane(index);

          plane.Transform(aTob);

          Real separation = ProjectPlaneOnHull(plane, hullB);
          if (separation > maxSeparation)
          {
            maxIndex = (int)index;
            maxSeparation = separation;
          }
        }

        faceQuery.m_index = maxIndex;
        faceQuery.m_separation = maxSeparation;
      }

      void QueryEdgeDirections(const Vector3 &posA, const Vector3 &scaleA, const Quaternion &orientA,
        const ColliderGeometry *hullA, const Vector3 &posB, const Vector3 &scaleB,
        const Quaternion &orientB, const ColliderGeometry *hullB, EdgeQuery &edgeQuery)
      {
        int maxIndexA = -1;
        int maxIndexB = -1;
        Real maxSeparation = -SF_REAL_MAX;

        Matrix4 ma, mb, AtoB;
        ma.SetTransformation(orientA, scaleA, posA);
        mb.SetTransformation(orientB, scaleB, posB);
        AtoB = mb.Inverted() * ma;

        Vector3 hullPos = AtoB * Vector3(0,0,0);
        Quaternion rotAtoB = orientB.Inverted() * orientA;

        for (int indexA = 0; indexA < hullA->GetEdgeCount(); indexA += 2)
        {
          const ColliderGeometry::HalfEdge &edgeA = hullA->GetEdge(indexA);
          const ColliderGeometry::HalfEdge &twinA = hullA->GetEdge(indexA + 1);
          SF_ASSERT(edgeA.m_twin == indexA + 1 && twinA.m_twin == indexA, 
            "Geometry is not correct: twin indecies do not match");

          Vector3 pa = AtoB * hullA->GetVertex(edgeA.m_origin);
          Vector3 qa = AtoB * hullA->GetVertex(twinA.m_origin);
          Vector3 ea = qa - pa;

          Vector3 ua = hullA->GetPlane(edgeA.m_face).Transformed(AtoB).GetNormal();
          Vector3 va = hullA->GetPlane(twinA.m_face).Transformed(AtoB).GetNormal();

          for (int indexB = 0; indexB < hullB->GetEdgeCount(); indexB += 2)
          {
            const ColliderGeometry::HalfEdge &edgeB = hullB->GetEdge(indexB);
            const ColliderGeometry::HalfEdge &twinB = hullB->GetEdge(indexB + 1);
            SF_ASSERT(edgeB.m_twin == indexB + 1 && twinB.m_twin == indexB,
              "Geometry is not correct: twin indecies do not match");

            Vector3 pb = hullB->GetVertex(edgeB.m_origin);
            Vector3 qb = hullB->GetVertex(twinB.m_origin);
            Vector3 eb = qb - pb;

            Vector3 ub = hullB->GetPlane(edgeB.m_face).GetNormal();
            Vector3 vb = hullB->GetPlane(twinB.m_face).GetNormal();

            if (IsMinkowskiFace(ua, va, -ea, -ub, -vb, -eb))
            {
              Real separation = ProjectEdge(pa, ea, pb, eb, hullPos);
              if (separation > maxSeparation)
              {
                if (indexA == 6 && indexB == 18)
                {
                  maxIndexA = indexA;
                  maxIndexB = indexB;
                }
                maxIndexA = indexA;
                maxIndexB = indexB;
                maxSeparation = separation;
              }
            }
          }
        }

        edgeQuery.m_edgeA = maxIndexA;
        edgeQuery.m_edgeB = maxIndexB;
        edgeQuery.m_separation = maxSeparation;
      }

      Real ProjectPlaneOnHull(const Geometry::Plane &plane, const ColliderGeometry *hull)
      {
        Vector3 support = hull->GetSupport(-plane.GetNormal());
        
        return plane.GetNormal().Dot(support) + plane.GetDistance();
      }

      Real ProjectEdge(const Vector3 &pa, const Vector3 &ea, const Vector3 &pb,
        const Vector3 &eb, const Vector3 &posA)
      {
        Vector3 aCrossb = ea.Cross(eb);

        static Real epsilon = Real(0.005);

        Real length = aCrossb.Length();
        if (length < epsilon * MathUtils::Sqrt(ea.LengthSq() * eb.LengthSq()))
          return -SF_REAL_MAX;
        
        Vector3 n = aCrossb / length;
        if (n.Dot(pa - posA) < Real(0.0))
          n.Negate();

        return n.Dot(pb - pa);
      }

      bool IsMinkowskiFace(const Vector3 &a, const Vector3 &b, const Vector3 &BxA,
        const Vector3 &c, const Vector3 &d, const Vector3 &DxC)
      {
        Real cba = c.Dot(BxA);
        Real dba = d.Dot(BxA);
        Real adc = a.Dot(DxC);
        Real bdc = b.Dot(DxC);

        return cba * dba < Real(0.0) && adc * bdc < Real(0.0) && cba * bdc > Real(0.0);
      }
    }
    //void SAT::BoxToBox(Contacts &contacts, ColliderData *colliderA,
    //  ColliderData *colliderB) const
    //{
    //  Transform *transA = SF_GET_COMP_TYPE(Transform, colliderA->m_transformHndl);
    //  Transform *transB = SF_GET_COMP_TYPE(Transform, colliderB->m_transformHndl);

    //  SF_ASSERT(transA, "Transform handle on collider A is not set");
    //  SF_ASSERT(transB, "Transform handle on collider B is not set");

    //  Vector3 posA = transA->GetTranslation() + colliderA->m_offset;
    //  Vector3 posB = transB->GetTranslation() + colliderB->m_offset;

    //  Vector3 scaleA = transA->GetScale() * colliderA->m_scale;
    //  Vector3 scaleB = transB->GetScale() * colliderB->m_scale;

    //  Quaternion orientA = transA->GetRotation();
    //  Quaternion orientB = transB->GetRotation();

    //  Matrix4 transMatA, transMatB;
    //  transMatA.SetTransformation(orientA, scaleA, posA);
    //  transMatB.SetTransformation(orientB, scaleB, posB);

    //  const ColliderGeometry &boxGeometry = GeometryMap::Instance()->GetBoxGeometry();

    //  const ColliderGeometry::VertexList& localVerts = boxGeometry.GetVertices();
    //  size_t numVerts = localVerts.size();

    //  std::vector<Vector3> worldVertsA;
    //  worldVertsA.resize(numVerts);
    //  std::vector<Vector3> worldVertsB;
    //  worldVertsB.resize(numVerts);

    //  for (size_t vert = 0; vert < numVerts; ++vert)
    //  {
    //    worldVertsA[vert] = transMatA * localVerts[vert];
    //    worldVertsB[vert] = transMatB * localVerts[vert];
    //  }

    //  Vector3 penetAxis;
    //  Real penetration;

    //  if (_FindSeparatingAxis(worldVertsA, worldVertsB, boxGeometry, boxGeometry, posA,
    //    posB, orientA, orientB, penetAxis, penetration))
    //    return;

    //  auto contactPoints = _GenerateContact(boxGeometry, boxGeometry, worldVertsA,
    //    worldVertsB, posA, posB, orientA, orientB, penetAxis);

    //  for (auto it : contactPoints)
    //  {
    //    Contact c;
    //    c.m_colliderA = colliderA;
    //    c.m_colliderB = colliderB;
    //    c.m_contactNormal = penetAxis;
    //    c.m_contactPoint = it;
    //    c.m_penetration = penetration;

    //    contacts.push_back(c);
    //  }

    //  return;
    //}

    //bool SAT::_FindSeparatingAxis(const std::vector<Vector3> &worldVertsA,
    //  const std::vector<Vector3> &worldVertsB, const ColliderGeometry &colGeomA,
    //  const ColliderGeometry &colGeomB, const Vector3 &posA, const Vector3 &posB,
    //  const Quaternion &orientA, const Quaternion &orientB, Vector3 &penetAxis, Real &minPenetration) const
    //{
    //  minPenetration = SF_REAL_MAX;

    //  //Check edges
    //  const ColliderGeometry::EdgeList& edgesA = colGeomA.GetUniqueEdges();
    //  const ColliderGeometry::EdgeList& edgesB = colGeomB.GetUniqueEdges();
    //  size_t numEdgesA = edgesA.size();
    //  size_t numEdgesB = edgesB.size();

    //  for (size_t edgeA = 0; edgeA < numEdgesA; ++edgeA)
    //  {
    //    ColliderGeometry::Edge eA = edgesA[edgeA];
    //    Vector3 evA = worldVertsA[eA.m_v0] - worldVertsA[eA.m_v1];
    //    orientA.Rotate(evA);

    //    for (size_t edgeB = 0; edgeB < numEdgesB; ++edgeB)
    //    {
    //      ColliderGeometry::Edge eB = edgesB[edgeB];
    //      Vector3 evB = worldVertsB[eB.m_v0] - worldVertsB[eB.m_v1];
    //      orientB.Rotate(evB);

    //      Vector3 edgeNormal = Cross(evA, evB);
    //      if (!edgeNormal.IsZeroEpsilon())
    //      {
    //        edgeNormal.Normalize();
    //        if (Dot(posA - posB, edgeNormal) > Real(0.0))
    //          edgeNormal.Negate();

    //        Real penetration;
    //        if (_IsAxisSeparating(worldVertsA, worldVertsB, posA, posB, 
    //          orientA, orientB, edgeNormal, penetration))
    //          return true;

    //        if (minPenetration > penetration)
    //        {
    //          minPenetration = penetration;
    //          penetAxis = edgeNormal;
    //        }
    //      }
    //    }
    //  }

    //  //Check faces
    //  const ColliderGeometry::FaceList& facesA = colGeomA.GetUniqueFaces();
    //  const ColliderGeometry::FaceList& facesB = colGeomB.GetUniqueFaces();
    //  size_t numFacesA = facesA.size();
    //  size_t numFacesB = facesB.size();

    //  for (size_t faceA = 0; faceA < numFacesA; ++faceA)
    //  {
    //    //Face normal assumed to be normalized
    //    Vector3 worldFaceNormal = orientA.Rotated(facesA[faceA].m_normal);
    //    if (Dot(posA - posB, worldFaceNormal) < Real(0.0))
    //      worldFaceNormal.Negate();

    //    Real penetration;
    //    if (_IsAxisSeparating(worldVertsA, worldVertsB, posA, posB,
    //      orientA, orientB, worldFaceNormal, penetration))
    //      return true;

    //    if (minPenetration > penetration)
    //    {
    //      minPenetration = penetration;
    //      penetAxis = worldFaceNormal;
    //    }
    //  }

    //  for (size_t faceB = 0; faceB < numFacesB; ++faceB)
    //  {
    //    //Face normal assumed to be normalized
    //    Vector3 worldFaceNormal = orientB.Rotated(facesB[faceB].m_normal);
    //    if (Dot(posA - posB, worldFaceNormal) < Real(0.0))
    //      worldFaceNormal.Negate();

    //    Real penetration;
    //    if (_IsAxisSeparating(worldVertsA, worldVertsB, posA, posB,
    //      orientA, orientB, worldFaceNormal, penetration))
    //      return true;

    //    if (minPenetration > penetration)
    //    {
    //      minPenetration = penetration;
    //      penetAxis = worldFaceNormal;
    //    }
    //  }

    //  if ((Dot(-(posA - posB), penetAxis))<0.0f)
    //    penetAxis.Negate();

    //  return false;
    //}

    //bool SAT::_IsAxisSeparating(const std::vector<Vector3> &worldVertsA,
    //  const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
    //  const Quaternion &orientA, const Quaternion &orientB, const Vector3 &axis, 
    //  Real &penetration) const
    //{
    //  Projection projA = _ProjectOnAxis(worldVertsA, posA, orientA, axis);
    //  Projection projB = _ProjectOnAxis(worldVertsB, posB, orientB, axis);

    //  if (!_IsOverlaping(projA, projB))
    //    return true;

    //  penetration = _CalculatePenetration(projA, projB);

    //  return false;
    //}

    //SAT::Projection SAT::_ProjectOnAxis(const std::vector<Vector3> &worldVerts, 
    //  const Vector3 &pos, const Quaternion &orient, const Vector3 &axis) const
    //{
    //  Projection proj;
    //  //Real offset = Dot(pos, axis);
    //  //Vector3 localAxis = orient.Inverted().Rotated(axis);

    //  size_t numVerts = worldVerts.size();
    //  for (size_t vert = 0; vert < numVerts; ++vert)
    //  {
    //    Real p = Dot(axis, worldVerts[vert]);
    //    if (p < proj.m_min)
    //      proj.m_min = p;
    //    if (p > proj.m_max)
    //      proj.m_max = p;
    //  }

    //  /*if (proj.m_min > proj.m_max)
    //    std::swap(proj.m_min, proj.m_max);
    //  
    //  proj.m_min += offset;
    //  proj.m_max += offset;*/

    //  return proj;
    //}

    //bool SAT::_IsOverlaping(const Projection &p1, const Projection &p2) const
    //{
    //  if (p1.m_max < p2.m_min || p1.m_min > p2.m_max)
    //    return false;
    //  return true;
    //}

    //Real SAT::_CalculatePenetration(const Projection &p1, const Projection &p2) const
    //{
    //  SF_ASSERT(p1.m_max - p2.m_min >= Real(0.0), "Penetration is negative");
    //  SF_ASSERT(p2.m_max - p1.m_min >= Real(0.0), "Penetration is negative");

    //  return std::min(p1.m_max - p2.m_min, p2.m_max - p1.m_min);
    //}

    //std::vector<Vector3> SAT::_GenerateContact(const ColliderGeometry &colGeomA,
    //  const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA, 
    //  const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
    //  const Quaternion &orientA, const Quaternion &orientB, 
    //  const Vector3 &contactNormal) const
    //{
    //  std::vector<Vector3> contactPoints;

    //  _ClipConHullToConHull(colGeomA, colGeomB, worldVertsA, worldVertsB, posA,
    //    posB, orientA, orientB, contactNormal, contactPoints);

    //  return contactPoints;
    //}

    //void SAT::_ClipConHullToConHull(const ColliderGeometry &colGeomA,
    //  const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA,
    //  const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
    //  const Quaternion &orientA, const Quaternion &orientB, const Vector3 &contactNormal, 
    //  std::vector<Vector3> &contactPoints) const
    //{
    //  Real minDist = SF_REAL_MAX;
    //  Real maxDist = -SF_REAL_MAX;
    //  size_t closestAInd = UINT64_MAX;
    //  size_t closestBInd = UINT64_MAX;

    //  const ColliderGeometry::FaceList& facesA = colGeomA.GetFaces();
    //  const ColliderGeometry::FaceList& facesB = colGeomB.GetFaces();
    //  size_t numFacesA = facesA.size();
    //  size_t numFacesB = facesB.size();

    //  for (size_t faceB = 0; faceB < numFacesB; ++faceB)
    //  {
    //    Vector3 faceNormalWorld = orientB.Rotated(facesB[faceB].m_normal);

    //    Real dist = Dot(contactNormal, faceNormalWorld);
    //    if (dist < minDist)
    //    {
    //      minDist = dist;
    //      closestBInd = faceB;
    //    }
    //  }

    //  SF_ASSERT(closestBInd != UINT64_MAX, "Closest face is not set");

    //  std::vector<Vector3> faceVerts;
    //  for (auto it : facesB[closestBInd].m_vertIndecies)
    //    faceVerts.push_back(worldVertsB[it]);

    //  for (size_t faceA = 0; faceA < numFacesA; ++faceA)
    //  {
    //    Vector3 faceNormalWorld = orientA.Rotated(facesA[faceA].m_normal);

    //    Real dist = Dot(contactNormal, faceNormalWorld);
    //    if (dist > maxDist)
    //    {
    //      maxDist = dist;
    //      closestAInd = faceA;
    //    }
    //  }

    //  if (closestAInd == UINT64_MAX)
    //    return;

    //  ColliderGeometry::Face closestA = facesA[closestAInd];
    //  size_t numVertsFaceA = closestA.m_vertIndecies.size();
    //  for (size_t faceVertA = 0; faceVertA < numVertsFaceA; ++faceVertA)
    //  {
    //    const Vector3 &v0 = worldVertsA[ closestA.m_vertIndecies[faceVertA] ];
    //    const Vector3 &v1 = worldVertsA[ 
    //      closestA.m_vertIndecies[(faceVertA + 1 < numVertsFaceA) ? faceVertA + 1 : 0] ];

    //    Vector3 worldEdge = v0 - v1;
    //    Vector3 clipPlaneNormal = -Cross(worldEdge, orientA.Rotated(closestA.m_normal));
    //    Real planeW = -Dot(v0, clipPlaneNormal);

    //    _ClipPointsToPlane(faceVerts, clipPlaneNormal, planeW);

    //    if (faceVerts.size() == 0)
    //      return;
    //  }

    //  size_t numClippedPts = faceVerts.size();
    //  Vector3 faceANormalWorld = orientA.Rotated(closestA.m_normal);
    //  Geometry::Plane planeA(faceANormalWorld, worldVertsA[closestA.m_vertIndecies[0]]);
    //    //- Dot(faceANormalWorld, posA);
    //  for (size_t i = 0; i < numClippedPts; ++i)
    //  {
    //    Geometry::IntersectionType::Type type = 
    //      Geometry::PointPlane(faceVerts[i], planeA.m_data, SF_EPSILON);

    //    if (type == Geometry::IntersectionType::Outside)
    //    {
    //      contactPoints.push_back(faceVerts[i]);
    //    }
    //  }
    //}

    //void SAT::_ClipPointsToPlane(std::vector<Vector3> &verts, const Vector3 &plane, 
    //  Real planeW) const
    //{
    //  std::vector<Vector3> clippedVerts;
    //  if (verts.size() < 2)
    //  {
    //    verts.clear();
    //    return;
    //  }

    //  Vector3 v0 = verts.back();
    //  Vector3 v1;

    //  Real startDist = Dot(plane, v0) + planeW;
    //  Real endDist;
    //  for (size_t vi = 0; vi < verts.size(); ++vi)
    //  {
    //    v1 = verts[vi];
    //    endDist = Dot(plane, v1) + planeW;

    //    if (startDist < 0)
    //    {
    //      if (endDist < 0)
    //        clippedVerts.push_back(v1);
    //      else
    //      {
    //        Real t = startDist / (startDist - endDist);
    //        SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
    //        clippedVerts.push_back(Lerp(v0, v1, t));
    //      }
    //    }
    //    else
    //    {
    //      if (endDist < 0)
    //      {
    //        Real t = startDist / (startDist - endDist);
    //        SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
    //        clippedVerts.push_back(Lerp(v0, v1, t));
    //        clippedVerts.push_back(v1);
    //      }
    //    }

    //    startDist = endDist;
    //    v0 = v1;
    //  }

    //  verts = clippedVerts;
    //}
  }
}