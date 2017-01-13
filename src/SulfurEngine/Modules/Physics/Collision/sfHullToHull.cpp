/******************************************************************************/
/*!
\par     Sulfur
\file    sfHullToHull.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfHullToHull.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "sfSAT.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"

namespace Sulfur
{
  namespace Physics
  {
    static void BuildEdgeContact(Contacts &contacts, ColliderData *colliderA, 
      const Vector3 &posA, const Vector3 &scaleA, const Quaternion &orientA,
      ColliderData *colliderB, const Vector3 &posB, const Vector3 &scaleB, 
      const Quaternion &orientB, const SAT::EdgeQuery &edgeQuery)
    {
      const ColliderGeometry *hullA = colliderA->m_geometry;
      const ColliderGeometry *hullB = colliderB->m_geometry;

      const ColliderGeometry::HalfEdge ea = hullA->GetEdge(edgeQuery.m_edgeA);
      Vector3 pa = hullA->GetVertex(ea.m_origin);
      Vector3 qa = hullA->GetVertex(hullA->GetEdge(ea.m_twin).m_origin);

      const ColliderGeometry::HalfEdge eb = hullB->GetEdge(edgeQuery.m_edgeB);
      Vector3 pb = hullB->GetVertex(eb.m_origin);
      Vector3 qb = hullB->GetVertex(hullB->GetEdge(eb.m_twin).m_origin);

      pa = posA + orientA.Rotated(scaleA * pa);
      qa = posA + orientA.Rotated(scaleA * qa);
      pb = posB + orientB.Rotated(scaleB * pb);
      qb = posB + orientB.Rotated(scaleB * qb);

      Vector3 normal = (qa - pa).Cross(qb - pb);

      Vector3 closestA, closestB;
      Geometry::ClosestPointsSegmentSegment(pa, qa, pb, qb, closestA, closestB);

      Real d = normal.Dot(posA - posB);
      if (d > Real(0.0))
        normal.Negate();

      Contact c;

      c.m_colliderA = colliderA;
      c.m_colliderB = colliderB;
      c.m_contactNormal = normal.Normalize();
      c.m_penetration = -edgeQuery.m_separation;
      c.m_contactPoint = closestA;

      SF_ASSERT(c.m_penetration < 0.7, "");

      contacts.push_back(c);
    }

    static void BuildFaceContact(Contacts &contacts, ColliderData *colliderRef,
      const Vector3 &posRef, const Vector3 &scaleRef, const Quaternion &orientRef,
      ColliderData *colliderInc, const Vector3 &posInc, const Vector3 &scaleInc,
      const Quaternion &orientInc, size_t refFace, size_t incFace, Real separation)
    {
      const ColliderGeometry *refHull = colliderRef->m_geometry;
      const ColliderGeometry *incHull = colliderInc->m_geometry;

      const ColliderGeometry::Face &iFace = incHull->GetFace(incFace);
      size_t firstEdge = iFace.m_edge;
      std::vector<Vector3> iVerts;
      
      size_t edgeIt = firstEdge;
      do
      {
        const ColliderGeometry::HalfEdge e = incHull->GetEdge(edgeIt);
        iVerts.push_back(incHull->GetVertex(e.m_origin));
        edgeIt = e.m_next;
      } while (edgeIt != firstEdge);

      for (auto &it : iVerts)
        it = posInc + orientInc.Rotated(it * scaleInc);

      Matrix4 refTransform;
      refTransform.SetTransformation(orientRef, scaleRef, posRef);

      const ColliderGeometry::Face &rFace = refHull->GetFace(refFace);
      firstEdge = rFace.m_edge;
      edgeIt = firstEdge;

      do
      {
        const ColliderGeometry::HalfEdge e = refHull->GetEdge(edgeIt);
        const ColliderGeometry::HalfEdge twin = refHull->GetEdge(e.m_twin);

        Geometry::Plane clipPlane = refHull->GetPlane(twin.m_face);
        clipPlane.Transform(refTransform);

        SAT::ClipPointsToPlane(iVerts, clipPlane);
        edgeIt = e.m_next;

      } while (edgeIt != firstEdge);

      Geometry::Plane refPlane = refHull->GetPlane(refFace);
      refPlane.Transform(refTransform);

      for (auto &it = iVerts.begin(); it != iVerts.end();)
      {
        Real d = refPlane.GetNormal().Dot(*it) + refPlane.GetDistance();
        if (d >= -separation)
          it = iVerts.erase(it);
        else //Behind, Outside
          ++it;
      }
      for (auto &it : iVerts)
      {
        Contact c;

        c.m_colliderA = colliderRef;
        c.m_colliderB = colliderInc;
        c.m_contactNormal = refPlane.GetNormal();
        c.m_contactPoint = it;
        c.m_penetration =
          (Geometry::ProjectPointOnPlane(it, refPlane.GetNormal(), refPlane.GetDistance()) - it).Length();

        SF_ASSERT(c.m_penetration < 1.1, "");

        contacts.push_back(c);
      }
    }

    static size_t GetIncidentFaceIndex(const size_t &refFaceInd, ColliderData *colliderRef,
      const Vector3 &posRef, const Vector3 &scaleRef, const Quaternion &orientRef,
      ColliderData *colliderInc, const Vector3 &posInc, const Vector3 &scaleInc,
      const Quaternion &orientInc)
    {
      Matrix4 refMat, incMat, refToInc;
      refMat.SetTransformation(orientRef, scaleRef, posRef);
      incMat.SetTransformation(orientInc, scaleInc, posInc);
      refToInc = incMat.Inverted() * refMat;

      const ColliderGeometry *refHull = colliderRef->m_geometry;
      const ColliderGeometry *incHull = colliderInc->m_geometry;

      Geometry::Plane refPlane = refHull->GetPlane(refFaceInd);
      refPlane.Transform(refToInc);

      const auto &incPlanes = incHull->GetPlanes();
      Real minDot = SF_REAL_MAX;
      size_t minIndex = 0;
      for (size_t i = 0; i < incPlanes.size(); ++i)
      {
        Real dot = refPlane.GetNormal().Dot(incPlanes[i].GetNormal());
        if (dot < minDot)
        {
          minDot = dot;
          minIndex = i;
        }
      }

      return minIndex;
    }

    void HullToHull(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB)
    {
      Transform *transA = SF_GET_COMP_TYPE(Transform, colliderA->m_transformHndl);
      Transform *transB = SF_GET_COMP_TYPE(Transform, colliderB->m_transformHndl);

      Vector3 posA = transA->GetTranslation() + colliderA->m_offset;
      Vector3 posB = transB->GetTranslation() + colliderB->m_offset;

      Vector3 scaleA = transA->GetScale() * colliderA->m_scale;
      Vector3 scaleB = transB->GetScale() * colliderB->m_scale;

      Quaternion orientA = transA->GetRotation();
      Quaternion orientB = transB->GetRotation();

      /*if (colliderB->m_compHndl == 17 && colliderA->m_compHndl == 0)
        SF_LOG_MESSAGE("Hit");*/

      SAT::FaceQuery faceQueryA;
      SAT::QueryFaceDirections(posA, scaleA, orientA, colliderA->m_geometry,
        posB, scaleB, orientB, colliderB->m_geometry, faceQueryA);
      if (faceQueryA.m_separation > Real(0.0))
        return;

      SAT::FaceQuery faceQueryB;
      SAT::QueryFaceDirections(posB, scaleB, orientB, colliderB->m_geometry,
        posA, scaleA, orientA, colliderA->m_geometry, faceQueryB);
      if (faceQueryB.m_separation > Real(0.0))
        return;

      SAT::EdgeQuery edgeQuery;
      SAT::QueryEdgeDirections(posA, scaleA, orientA, colliderA->m_geometry,
        posB, scaleB, orientB, colliderB->m_geometry, edgeQuery);
      if (edgeQuery.m_separation > Real(0.0))
        return;

      if (edgeQuery.m_separation > faceQueryA.m_separation
        && edgeQuery.m_separation > faceQueryB.m_separation)
      {
        if (edgeQuery.m_separation < -0.4)
        {
          if (edgeQuery.m_edgeA == 6 && edgeQuery.m_edgeB == 18)
            edgeQuery.m_separation = edgeQuery.m_separation;
        }

        BuildEdgeContact(contacts, colliderA, posA, scaleA, orientA, colliderB,
          posB, scaleB, orientB, edgeQuery);
      }
      else
      {
        if (faceQueryA.m_separation > faceQueryB.m_separation)
        {
          size_t incFace = GetIncidentFaceIndex(faceQueryA.m_index, 
            colliderA, posA, scaleA, orientA, colliderB, posB, scaleB, orientB);

          BuildFaceContact(contacts, colliderA, posA, scaleA, orientA,
            colliderB, posB, scaleB, orientB, faceQueryA.m_index, incFace,
            faceQueryA.m_separation);
        }
        else
        {
          size_t incFace = GetIncidentFaceIndex(faceQueryB.m_index,
            colliderB, posB, scaleB, orientB, colliderA, posA, scaleA, orientA);

          BuildFaceContact(contacts, colliderB, posB, scaleB, orientB,
            colliderA, posA, scaleA, orientA, faceQueryB.m_index, incFace,
            faceQueryB.m_separation);
        }
      }
    }
  }
}