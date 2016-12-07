#include "sfSphereToBox.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"
#include "Modules\Physics\Collision\sfGJK.hpp"
#include "Math\Geometry\sfGeometry.hpp"
#include "sfSAT.hpp"
#include "Math\Geometry\sfShapes.hpp"

namespace Sulfur
{
  namespace Physics
  {
    size_t FindClosestFace(const ColliderGeometry &geometry,
      const Matrix4 &trans, const Vector3 &contactNormal)
    {
      Real minDist = SF_REAL_MAX;
      size_t ind = 0;
      for (size_t i = 0; i < geometry.GetFaceCount(); ++i)
      {
        Real dist = -Dot(contactNormal, geometry.GetPlane(i).Transformed(trans).GetNormal());

        if (dist < minDist)
        {
          minDist = dist;
          ind = i;
        }
      }

      return ind;
    }

    static void BuildDeepContact(Contacts &contacts, ColliderData *box,
      ColliderData *capsule, const Vector3 &boxPos, const Vector3 &boxScale,
      const Quaternion &boxOrient, const Vector3 &capLineA, const Vector3 &capLineB,
      Real capRadius, const Gjk::CsoPoint &closestPts)
    {
      const ColliderGeometry &boxGeometry = GeometryMap::Instance()->GetBoxGeometry();

      ColliderGeometry::VertexList worldCapVerts;
      worldCapVerts.push_back(capLineA);
      worldCapVerts.push_back(capLineB);
      Matrix4 trans;
      trans.SetTransformation(boxOrient, boxScale, boxPos);

      int faceContact = -1;
      Real minDist = SF_REAL_MAX;
      Real maxFace = -SF_REAL_MAX;
      Vector3 minAxis;

      for (int i = 0; i < boxGeometry.GetPlaneCount(); ++i)
      {
        Geometry::Plane p = boxGeometry.GetPlane(i);
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

      for (size_t index = 0; index < boxGeometry.GetEdgeCount(); index += 2)
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

        auto projLine = SAT::ProjectOnAxis(worldCapVerts, crossEP);

        Real dist = projLine.m_max - projLine.m_min;
        if (dist < minDist)
        {
          minDist = dist;
          minAxis = crossEP;
          faceContact = -1;
        }
      }

      if (faceContact != -1)
      {
        size_t firstEdge = boxGeometry.GetFace(faceContact).m_edge;
        size_t edgeIt = firstEdge;

        do
        {
          const ColliderGeometry::HalfEdge &e = boxGeometry.GetEdge(edgeIt);
          const ColliderGeometry::HalfEdge &twin = boxGeometry.GetEdge(e.m_twin);
          Geometry::Plane plane = boxGeometry.GetPlane(twin.m_face);
          plane.Transform(trans);

          SAT::ClipLineToPlane(worldCapVerts[0], worldCapVerts[1], plane.GetNormal(), plane.GetDistance());

          edgeIt = e.m_next;
        } while (firstEdge != edgeIt);

        Geometry::Plane plane = boxGeometry.GetPlane(faceContact);
        plane.Transform(trans);
        for (size_t i = 0; i < worldCapVerts.size(); ++i)
        {
          Real dist = plane.GetNormal().Dot(worldCapVerts[i]) + plane.GetDistance();
          
          Contact c;

          c.m_colliderA = box;
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

        c.m_colliderA = box;
        c.m_colliderB = capsule;
        c.m_contactNormal = minAxis;
        c.m_contactPoint = closestPts.m_pointB;
        c.m_penetration = minDist;

        contacts.push_back(c);

        return;
      }
    }

    void BoxToCapsule(Contacts &contacts, ColliderData *box,
      ColliderData *capsule)
    {
      Transform *boxTrans = SF_GET_COMP_TYPE(Transform, box->m_transformHndl);
      Transform *capTrans = SF_GET_COMP_TYPE(Transform, capsule->m_transformHndl);

      Vector3 boxScale = boxTrans->GetScale() * box->m_scale;
      Vector3 boxPos = boxTrans->GetTranslation() + box->m_offset;

      Vector3 capPos = capTrans->GetTranslation() + capsule->m_offset;
      Real cLineLength = capsule->m_lineLength * capTrans->GetScale()[0];
      Real capRadius = capsule->m_radius *
        std::max(capTrans->GetScale()[1], capTrans->GetScale()[2]);

      PointsSupportShape capLineShape;
      capLineShape.m_localSpacePoints.push_back(Vector3(Real(-0.5), Real(0.0), Real(0.0)));
      capLineShape.m_localSpacePoints.push_back(Vector3(Real(0.5), Real(0.0), Real(0.0)));
      capLineShape.m_scale = Vector3(cLineLength, 1, 1);
      capLineShape.m_rotation = capTrans->GetRotation();
      capLineShape.m_translation = capPos;

      ObbSupportShape boxShape;
      boxShape.m_scale = boxScale;
      boxShape.m_rotation = boxTrans->GetRotation();
      boxShape.m_translation = boxPos;

      Gjk::CsoPoint pointInfo;

      Vector3 capLineA, capLineB;
      capLineA = capPos +
        capLineShape.m_rotation.Rotated(capLineShape.m_scale * Vector3(Real(-0.5), Real(0.0), Real(0.0)));
      capLineB = capPos +
        capLineShape.m_rotation.Rotated(capLineShape.m_scale * Vector3(Real(0.5), Real(0.0), Real(0.0)));

      Matrix4 trans;
      trans.SetTransformation(boxTrans->GetRotation(), boxScale, boxPos);

      std::vector<Gjk::CsoPoint> simplex;
      bool intersect = Gjk::Intersect(&capLineShape, &boxShape, 4, pointInfo, SF_EPSILON, simplex);

      if (intersect) //Deep contact
      {
        BuildDeepContact(contacts, box, capsule, boxPos, boxScale, boxTrans->GetRotation(),
          capLineA, capLineB, capRadius, pointInfo);
        return;
      }

      Vector3 CtoB = pointInfo.m_pointA - pointInfo.m_pointB;
      Real dist = CtoB.Length();

      if (dist > capRadius)
        return;

      CtoB.Normalize();

      const ColliderGeometry &boxGeometry = GeometryMap::Instance()->GetBoxGeometry();
      const ColliderGeometry::FaceList &faceList = boxGeometry.GetFaces();

      Vector3 lineVec = capLineA - capLineB;
      size_t refFace = FindClosestFace(boxGeometry, trans, CtoB);
      Geometry::Plane refPlane = boxGeometry.GetPlane(refFace).Transformed(trans);
      Vector3 worldFaceNormal = refPlane.GetNormal();
      Real dot = MathUtils::Abs(worldFaceNormal.Dot(lineVec));

      if (dot <= 0.005) //We need two points here
      {
        size_t firstEdge = boxGeometry.GetFace(refFace).m_edge;
        size_t edgeIt = firstEdge;

        do
        {
          size_t twin = boxGeometry.GetEdge(edgeIt).m_twin;
          Geometry::Plane clipPlane = boxGeometry.GetPlane(boxGeometry.GetEdge(twin).m_face);
          clipPlane.Transformed(trans);

          SAT::ClipLineToPlane(capLineA, capLineB, clipPlane.GetNormal(), clipPlane.GetDistance());

          edgeIt = boxGeometry.GetEdge(edgeIt).m_next;
        } while (firstEdge != edgeIt);

        Contact c0;

        c0.m_colliderA = box;
        c0.m_colliderB = capsule;
        c0.m_contactPoint = 
          Geometry::ProjectPointOnPlane(capLineA, refPlane.GetNormal(), refPlane.GetDistance());
        c0.m_contactNormal = CtoB;
        c0.m_penetration = capRadius - dist;

        contacts.push_back(c0);

        Contact c1;

        c1.m_colliderA = box;
        c1.m_colliderB = capsule;
        c1.m_contactPoint = 
          Geometry::ProjectPointOnPlane(capLineB, refPlane.GetNormal(), refPlane.GetDistance());
        c1.m_contactNormal = CtoB;
        c1.m_penetration = capRadius - dist;

        contacts.push_back(c1);
      }
      else
      {
        Contact c;

        c.m_colliderA = box;
        c.m_colliderB = capsule;
        c.m_contactPoint = pointInfo.m_pointB;
        c.m_contactNormal = (pointInfo.m_pointA - pointInfo.m_pointB).Normalized();
        c.m_penetration = capRadius - dist;

        contacts.push_back(c);
      }
    }
  }
}