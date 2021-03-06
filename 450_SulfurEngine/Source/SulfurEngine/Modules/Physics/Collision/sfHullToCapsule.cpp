/******************************************************************************/
/*!
\par     Sulfur
\file    sfBoxToCapsule.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"
#include "Modules\Physics\Collision\sfGJK.hpp"
#include "Math\Geometry\sfGeometry.hpp"
#include "sfSAT.hpp"
#include "Math\Geometry\sfShapes.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"

namespace Sulfur
{
  namespace Physics
  {
    size_t FindClosestFace(const ColliderGeometry *geometry,
      const Matrix4 &trans, const Vector3 &contactNormal)
    {
      Real minDist = SF_REAL_MAX;
      size_t ind = 0;
      for (size_t i = 0; i < geometry->GetFaceCount(); ++i)
      {
        Real dist = -Dot(contactNormal, geometry->GetPlane(i).Transformed(trans).GetNormal());

        if (dist < minDist)
        {
          minDist = dist;
          ind = i;
        }
      }

      return ind;
    }

    static void BuildDeepContact(Contacts &contacts, ColliderData *hull,
      ColliderData *capsule, const Vector3 &boxPos, const Vector3 &boxScale,
      const Quaternion &boxOrient, const Vector3 &capLineA, const Vector3 &capLineB,
      Real capRadius, const Gjk::CsoPoint &closestPts)
    {
      const ColliderGeometry *hullGeometry = hull->m_geometry;

      ColliderGeometry::VertexList worldCapVerts;
      worldCapVerts.push_back(capLineA);
      worldCapVerts.push_back(capLineB);
      Matrix4 trans;
      trans.SetTransformation(boxOrient, boxScale, boxPos);

      int faceContact = -1;
      Real minDist = SF_REAL_MAX;
      Real maxFace = -SF_REAL_MAX;
      Vector3 minAxis;

      for (int i = 0; i < hullGeometry->GetPlaneCount(); ++i)
      {
        Geometry::Plane p = hullGeometry->GetPlane(i);
        p.Transform(trans);
        
        Real da = p.GetNormal().Dot(capLineA) + p.GetDistance();
        Real db = p.GetNormal().Dot(capLineB) + p.GetDistance();

        Real dist = std::min(da, db);
        if (dist > maxFace)
        {
          maxFace = dist;
          minAxis = p.GetNormal();
          faceContact = i;
        }
      }

      /*for (size_t index = 0; index < boxGeometry.GetEdgeCount(); index += 2)
      {
        const ColliderGeometry::HalfEdge &edge = boxGeometry.GetEdge(index);
        const ColliderGeometry::HalfEdge &twin = boxGeometry.GetEdge(index + 1);

        Vector3 localEdge = boxGeometry.GetVertex(edge.m_origin)
          - boxGeometry.GetVertex(twin.m_origin);
        Vector3 worldEdge = boxOrient.Rotated(localEdge);

        Vector3 crossEP = worldEdge.Cross(capLineB - capLineA);

        if (crossEP.IsZeroEpsilon())
          continue;

        crossEP.Normalize();

        Real t = (capLineB - capLineA).Dot(trans * boxGeometry.GetVertex(edge.m_origin)) * 
          (capLineB - capLineA).Dot(trans * boxGeometry.GetVertex(twin.m_origin));

        auto projLine = SAT::ProjectOnAxis(worldCapVerts, crossEP);

        Real dist = projLine.m_max - projLine.m_min;
        if (dist < minDist)
        {
          minDist = dist;
          minAxis = crossEP;
          faceContact = -1;
        }
      }*/

      if (faceContact != -1)
      {
        size_t firstEdge = hullGeometry->GetFace(faceContact).m_edge;
        size_t edgeIt = firstEdge;

        do
        {
          const ColliderGeometry::HalfEdge &e = hullGeometry->GetEdge(edgeIt);
          const ColliderGeometry::HalfEdge &twin = hullGeometry->GetEdge(e.m_twin);
          Geometry::Plane plane = hullGeometry->GetPlane(twin.m_face);
          plane.Transform(trans);

          SAT::ClipLineToPlane(worldCapVerts[0], worldCapVerts[1], plane.GetNormal(), plane.GetDistance());

          edgeIt = e.m_next;
        } while (firstEdge != edgeIt);

        Geometry::Plane plane = hullGeometry->GetPlane(faceContact);
        plane.Transform(trans);
        for (size_t i = 0; i < worldCapVerts.size(); ++i)
        {
          Real dist = plane.GetNormal().Dot(worldCapVerts[i]) + plane.GetDistance();
          
          Contact c;

          c.m_colliderA = hull;
          c.m_colliderB = capsule;
          c.m_contactNormal = minAxis;
          c.m_contactPoint = Geometry::ProjectPointOnPlane(worldCapVerts[i], plane.GetNormal(), plane.GetDistance());
          c.m_penetration = MathUtils::Abs( dist - capRadius);

          contacts.push_back(c);
        }
      }
      else
      {

        //Not correct, but at least it does not look wierd and does not crash
        //Should work for now
        Contact c;

        c.m_colliderA = hull;
        c.m_colliderB = capsule;
        c.m_contactNormal = minAxis;
        c.m_contactPoint = closestPts.m_pointA;
        c.m_penetration = minDist;

        contacts.push_back(c);

        return;
      }
    }

    void HullToCapsule(Contacts &contacts, ColliderData *hull,
      ColliderData *capsule)
    {
      Transform *hullTrans = SF_GET_COMP_TYPE(Transform, hull->m_transformHndl);
      Transform *capTrans = SF_GET_COMP_TYPE(Transform, capsule->m_transformHndl);

      Vector3 boxScale = hullTrans->GetScale() * hull->m_scale;
      Vector3 boxPos = hullTrans->GetTranslation() + hull->m_offset;

      Vector3 capPos = capTrans->GetTranslation() + capsule->m_offset;
      Real cLineLength = capsule->m_lineLength * capTrans->GetScale()[0];
      Real capRadius = capsule->m_radius *
        std::max(capTrans->GetScale()[1], capTrans->GetScale()[2]);

      PointsSupportShape capLineShape;
      capLineShape.m_localSpacePoints.push_back(Vector3(Real(-0.5), Real(0.0), Real(0.0)));
      capLineShape.m_localSpacePoints.push_back(Vector3(Real(0.0), Real(0.0), Real(0.0)));
      capLineShape.m_localSpacePoints.push_back(Vector3(Real(0.5), Real(0.0), Real(0.0)));
      capLineShape.m_scale = Vector3(cLineLength, 1, 1);
      capLineShape.m_rotation = capTrans->GetRotation();
      capLineShape.m_translation = capPos;

      ObbSupportShape boxShape;
      boxShape.m_scale = boxScale;
      boxShape.m_rotation = hullTrans->GetRotation();
      boxShape.m_translation = boxPos;

      Gjk::CsoPoint pointInfo;

      Vector3 capLineA, capLineB;
      capLineA = capPos +
        capLineShape.m_rotation.Rotated(capLineShape.m_scale * Vector3(Real(-0.5), Real(0.0), Real(0.0)));
      capLineB = capPos +
        capLineShape.m_rotation.Rotated(capLineShape.m_scale * Vector3(Real(0.5), Real(0.0), Real(0.0)));

      Matrix4 trans;
      trans.SetTransformation(hullTrans->GetRotation(), boxScale, boxPos);

      std::vector<Gjk::CsoPoint> simplex;
      bool intersect = Gjk::Intersect(&boxShape, &capLineShape, 5, pointInfo, 0.005f, simplex);

      if (intersect) //Deep contact
      {
        BuildDeepContact(contacts, hull, capsule, boxPos, boxScale, hullTrans->GetRotation(),
          capLineA, capLineB, capRadius, pointInfo);
        return;
      }

      Vector3 CtoB = pointInfo.m_pointB - pointInfo.m_pointA;
      Real dist = CtoB.Length();

      if (dist > capRadius)
        return;

      CtoB.Normalize();

      const ColliderGeometry *hullGeometry = hull->m_geometry;
      const ColliderGeometry::FaceList &faceList = hullGeometry->GetFaces();

      Vector3 lineVec = capLineA - capLineB;
      size_t refFace = FindClosestFace(hullGeometry, trans, CtoB);
      Geometry::Plane refPlane = hullGeometry->GetPlane(refFace).Transformed(trans);
      Vector3 worldFaceNormal = refPlane.GetNormal();
      Real dot = MathUtils::Abs(worldFaceNormal.Dot(lineVec));

      if (dot <= 0.09) //We need two points here
      {
        size_t firstEdge = hullGeometry->GetFace(refFace).m_edge;
        size_t edgeIt = firstEdge;

        do
        {
          size_t twin = hullGeometry->GetEdge(edgeIt).m_twin;
          Geometry::Plane clipPlane = hullGeometry->GetPlane(hullGeometry->GetEdge(twin).m_face);
          clipPlane.Transform(trans);

          SAT::ClipLineToPlane(capLineA, capLineB, clipPlane.GetNormal(), clipPlane.GetDistance());

          edgeIt = hullGeometry->GetEdge(edgeIt).m_next;
        } while (firstEdge != edgeIt);

        Contact c0;

        c0.m_colliderA = hull;
        c0.m_colliderB = capsule;
        c0.m_contactPoint = 
          Geometry::ProjectPointOnPlane(capLineA, refPlane.GetNormal(), refPlane.GetDistance());
        c0.m_contactNormal = CtoB;
        c0.m_penetration = capRadius - (c0.m_contactPoint - capLineA).Length();

        contacts.push_back(c0);

        Contact c1;

        c1.m_colliderA = hull;
        c1.m_colliderB = capsule;
        c1.m_contactPoint = 
          Geometry::ProjectPointOnPlane(capLineB, refPlane.GetNormal(), refPlane.GetDistance());
        c1.m_contactNormal = CtoB;
        c1.m_penetration = capRadius - (c1.m_contactPoint - capLineB).Length();

        contacts.push_back(c1);
      }
      else
      {
        Contact c;

        c.m_colliderA = hull;
        c.m_colliderB = capsule;
        c.m_contactPoint = pointInfo.m_pointA;
        c.m_contactNormal = (pointInfo.m_pointB - pointInfo.m_pointA).Normalized();
        c.m_penetration = capRadius - dist;

        contacts.push_back(c);
      }
    }
  }
}