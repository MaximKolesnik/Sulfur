#include "sfSAT.hpp"
#include "Math\SpatialPartitions\sfISpatialPartition.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Math\sfQuaternion.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"

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
    void SAT::BoxToBox(Contacts &contacts, ColliderData *colliderA, 
      ColliderData *colliderB) const
    {
      Transform *transA = SF_GET_COMP_TYPE(Transform, colliderA->m_transformHndl);
      Transform *transB = SF_GET_COMP_TYPE(Transform, colliderB->m_transformHndl);

      SF_ASSERT(transA, "Transform handle on collider A is not set");
      SF_ASSERT(transB, "Transform handle on collider B is not set");

      Vector3 posA = transA->GetTranslation() + colliderA->m_offset;
      Vector3 posB = transB->GetTranslation() + colliderB->m_offset;

      Vector3 scaleA = transA->GetScale() * colliderA->m_scale;
      Vector3 scaleB = transB->GetScale() * colliderB->m_scale;

      Quaternion orientA = transA->GetRotation();
      Quaternion orientB = transB->GetRotation();

      Matrix4 transMatA, transMatB;
      transMatA.SetTransformation(orientA, scaleA, posA);
      transMatB.SetTransformation(orientB, scaleB, posB);

      const ColliderGeometry &boxGeometry = GeometryMap::Instance()->GetBoxGeometry();

      const ColliderGeometry::VertexList& localVerts = boxGeometry.GetVertices();
      size_t numVerts = localVerts.size();

      std::vector<Vector3> worldVertsA;
      worldVertsA.resize(numVerts);
      std::vector<Vector3> worldVertsB;
      worldVertsB.resize(numVerts);

      for (size_t vert = 0; vert < numVerts; ++vert)
      {
        worldVertsA[vert] = transMatA * localVerts[vert];
        worldVertsB[vert] = transMatB * localVerts[vert];
      }

      Vector3 penetAxis;
      Real penetration;

      if (_FindSeparatingAxis(worldVertsA, worldVertsB, boxGeometry, boxGeometry, posA,
        posB, orientA, orientB, penetAxis, penetration))
        return;

      auto contactPoints = _GenerateContact(boxGeometry, boxGeometry, worldVertsA, 
        worldVertsB, posA, posB, orientA, orientB, penetAxis);

      for (auto it : contactPoints)
      {
        Contact c;
        c.m_colliderA = colliderA;
        c.m_colliderB = colliderB;
        c.m_contactNormal = penetAxis;
        c.m_contactPoint = it;
        c.m_penetration = penetration;

        contacts.push_back(c);
        Matrix4 m;
        m.SetTransformation(Quaternion(Real(1.0), 0.0, 0.0, 0.0), 
          Vector3(Real(0.2), Real(0.2), Real(0.2)), it);
        DebugDraw::Instance()->DrawBox(m);
        DebugDraw::Instance()->DrawVector(it, penetAxis);
      }

      return;
    }

    bool SAT::_FindSeparatingAxis(const std::vector<Vector3> &worldVertsA,
      const std::vector<Vector3> &worldVertsB, const ColliderGeometry &colGeomA,
      const ColliderGeometry &colGeomB, const Vector3 &posA, const Vector3 &posB,
      const Quaternion &orientA, const Quaternion &orientB, Vector3 &penetAxis, Real &minPenetration) const
    {
      minPenetration = SF_REAL_MAX;

      Vector3 maxAxis;

      //Check edges
      const ColliderGeometry::EdgeList& edgesA = colGeomA.GetUniqueEdges();
      const ColliderGeometry::EdgeList& edgesB = colGeomB.GetUniqueEdges();
      size_t numEdgesA = edgesA.size();
      size_t numEdgesB = edgesB.size();

      for (size_t edgeA = 0; edgeA < numEdgesA; ++edgeA)
      {
        ColliderGeometry::Edge eA = edgesA[edgeA];
        Vector3 evA = worldVertsA[eA.m_v1] - worldVertsA[eA.m_v0];
        orientA.Rotate(evA);

        for (size_t edgeB = 0; edgeB < numEdgesB; ++edgeB)
        {
          ColliderGeometry::Edge eB = edgesB[edgeB];
          Vector3 evB = worldVertsB[eB.m_v1] - worldVertsB[eB.m_v0];
          orientB.Rotate(evB);

          Vector3 edgeNormal = Cross(evA, evB);
          if (!edgeNormal.IsZeroEpsilon())
          {
            edgeNormal.Normalize();
            if (Dot(posA - posB, edgeNormal) > Real(0.0))
              edgeNormal.Negate();

            Real penetration;
            if (_IsAxisSeparating(worldVertsA, worldVertsB, edgeNormal, penetration))
              return true;

            if (minPenetration > penetration)
            {
              minPenetration = penetration;
              penetAxis = edgeNormal;
            }
          }
        }
      }

      //Check faces
      const ColliderGeometry::FaceList& facesA = colGeomA.GetFaces();
      const ColliderGeometry::FaceList& facesB = colGeomB.GetFaces();
      size_t numFacesA = facesA.size();
      size_t numFacesB = facesB.size();

      for (size_t faceA = 0; faceA < numFacesA; ++faceA)
      {
        //Face normal assumed to be normalized
        Vector3 worldFaceNormal = orientA.Rotated(facesA[faceA].m_normal);
        if (Dot(posA - posB, worldFaceNormal) < Real(0.0))
          worldFaceNormal.Negate();

        Real penetration;
        if (_IsAxisSeparating(worldVertsA, worldVertsB, worldFaceNormal, penetration))
          return true;

        if (minPenetration > penetration)
        {
          minPenetration = penetration;
          penetAxis = worldFaceNormal;
        }
      }

      for (size_t faceB = 0; faceB < numFacesB; ++faceB)
      {
        //Face normal assumed to be normalized
        Vector3 worldFaceNormal = orientB.Rotated(facesB[faceB].m_normal);
        if (Dot(posA - posB, worldFaceNormal) < Real(0.0))
          worldFaceNormal.Negate();

        Real penetration;
        if (_IsAxisSeparating(worldVertsA, worldVertsB, worldFaceNormal, penetration))
          return true;

        if (minPenetration > penetration)
        {
          minPenetration = penetration;
          penetAxis = worldFaceNormal;
        }
      }

      if ((Dot(-(posA - posB), penetAxis))<0.0f)
        penetAxis.Negate();

      return false;
    }

    bool SAT::_IsAxisSeparating(const std::vector<Vector3> &worldVertsA,
      const std::vector<Vector3> &worldVertsB, const Vector3 &axis, Real &penetration) const
    {
      Projection projA = _ProjectOnAxis(worldVertsA, axis);
      Projection projB = _ProjectOnAxis(worldVertsB, axis);

      if (!_IsOverlaping(projA, projB))
        return true;

      penetration = _CalculatePenetration(projA, projB);

      return false;
    }

    SAT::Projection SAT::_ProjectOnAxis(const std::vector<Vector3> &worldVerts, 
      const Vector3 &axis) const
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

    bool SAT::_IsOverlaping(const Projection &p1, const Projection &p2) const
    {
      if (p1.m_max < p2.m_min || p1.m_min > p2.m_max)
        return false;
      return true;
    }

    Real SAT::_CalculatePenetration(const Projection &p1, const Projection &p2) const
    {
      SF_ASSERT(p1.m_max - p2.m_min >= Real(0.0), "Penetration is negative");
      SF_ASSERT(p2.m_max - p1.m_min >= Real(0.0), "Penetration is negative");

      return std::min(p1.m_max - p2.m_min, p2.m_max - p1.m_min);
    }

    std::vector<Vector3> SAT::_GenerateContact(const ColliderGeometry &colGeomA,
      const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA, 
      const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
      const Quaternion &orientA, const Quaternion &orientB, const Vector3 &contactNormal) const
    {
      std::vector<Vector3> contactPoints;

      _ClipConHullToConHull(colGeomA, colGeomB, worldVertsA, worldVertsB, posA,
        posB, orientA, orientB, contactNormal, contactPoints);

      return contactPoints;
    }

    void SAT::_ClipConHullToConHull(const ColliderGeometry &colGeomA,
      const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA,
      const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
      const Quaternion &orientA, const Quaternion &orientB, const Vector3 &contactNormal, 
      std::vector<Vector3> &contactPoints) const
    {
      Real minDist = SF_REAL_MAX;
      Real maxDist = -SF_REAL_MAX;
      size_t closestAInd = UINT64_MAX;
      size_t closestBInd = UINT64_MAX;

      const ColliderGeometry::FaceList& facesA = colGeomA.GetFaces();
      const ColliderGeometry::FaceList& facesB = colGeomB.GetFaces();
      size_t numFacesA = facesA.size();
      size_t numFacesB = facesB.size();

      for (size_t faceB = 0; faceB < numFacesB; ++faceB)
      {
        Vector3 faceNormalWorld = orientB.Rotated(facesB[faceB].m_normal);

        Real dist = Dot(contactNormal, faceNormalWorld);
        if (dist < minDist)
        {
          minDist = dist;
          closestBInd = faceB;
        }
      }

      SF_ASSERT(closestBInd != UINT64_MAX, "Closest face is not set");

      std::vector<Vector3> faceVerts;
      for (auto it : facesB[closestBInd].m_vertIndecies)
        faceVerts.push_back(worldVertsB[it]);

      for (size_t faceA = 0; faceA < numFacesA; ++faceA)
      {
        Vector3 faceNormalWorld = orientA.Rotated(facesA[faceA].m_normal);

        Real dist = Dot(contactNormal, faceNormalWorld);
        if (dist > maxDist)
        {
          maxDist = dist;
          closestAInd = faceA;
        }
      }

      if (closestAInd == UINT64_MAX)
        return;

      ColliderGeometry::Face closestA = facesA[closestAInd];
      size_t numVertsFaceA = closestA.m_vertIndecies.size();
      for (size_t faceVertA = 0; faceVertA < numVertsFaceA; ++faceVertA)
      {
        const Vector3 &v0 = worldVertsA[ closestA.m_vertIndecies[faceVertA] ];
        const Vector3 &v1 = worldVertsA[ 
          closestA.m_vertIndecies[(faceVertA + 1 < numVertsFaceA) ? faceVertA + 1 : 0] ];

        Vector3 worldEdge = v0 - v1;
        Vector3 clipPlaneNormal = -Cross(worldEdge, orientA.Rotated(closestA.m_normal));
        Real planeW = -Dot(v0, clipPlaneNormal);

        _ClipPointsToPlane(faceVerts, clipPlaneNormal, planeW);

        if (faceVerts.size() == 0)
          return;
      }

      size_t numClippedPts = faceVerts.size();
      Vector3 faceANormalWorld = orientA.Rotated(closestA.m_normal);
      Geometry::Plane planeA(faceANormalWorld, posA);
      for (size_t i = 0; i < numClippedPts; ++i)
      {
        Real dist = Dot(planeA.m_data, Vector4(faceVerts[i][0], faceVerts[i][1], faceVerts[i][2], Real(-1.0)));
        if (dist <= maxDist)
        {
          contactPoints.push_back(faceVerts[i]);
        }
      }
    }

    void SAT::_ClipPointsToPlane(std::vector<Vector3> &verts, const Vector3 &plane, 
      Real planeW) const
    {
      std::vector<Vector3> clippedVerts;

      Vector3 v0 = verts.back();
      Vector3 v1;

      Real startDist = Dot(plane, v0) + planeW;
      Real endDist;
      for (size_t vi = 0; vi < verts.size(); ++vi)
      {
        v1 = verts[vi];
        endDist = Dot(plane, v1) + planeW;

        if (startDist < 0)
        {
          if (endDist < 0)
            clippedVerts.push_back(v1);
          else
          {
            Real t = startDist / (startDist - endDist);
            SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
            clippedVerts.push_back(Lerp(v0, v1, t));
          }
        }
        else
        {
          if (endDist < 0)
          {
            Real t = startDist / (startDist - endDist);
            SF_ASSERT(t >= Real(0.0), "Lerp t value is negative");
            clippedVerts.push_back(Lerp(v0, v1, t));
            clippedVerts.push_back(v1);
          }
        }

        startDist = endDist;
        v0 = v1;
      }

      verts = clippedVerts;
    }
  }
}