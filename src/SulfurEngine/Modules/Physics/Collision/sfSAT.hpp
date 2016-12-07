#pragma once

#include "sfIContactGenerator.hpp"
#include "Math\sfMathDefs.hpp"
#include "Math\sfVector3.hpp"
#include "Math\sfQuaternion.hpp"
#include "Modules\Physics\ColliderGeometry\sfColliderGeometry.hpp"
#include "Math\Geometry\sfGeometry.hpp"
#include "Math\Geometry\sfShapes.hpp"
#include "Modules\Physics\Collision\sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    /*class SAT : public IContactGenerator
    {
    public:
      SAT(void) : IContactGenerator() {}
      virtual ~SAT(void) {}

      virtual void BoxToBox(Contacts &contacts, 
        ColliderData *colliderA, ColliderData *colliderB) const override final;
      virtual void SphereToSphere(Contacts &contacts,
        ColliderData *colliderA, ColliderData *colliderB) const override final;
      virtual void SphereToBox(Contacts &contacts, ColliderData *sphere,
        ColliderData *box) const override final;
      virtual void BoxToCapsule(Contacts &contacts, ColliderData *box,
        ColliderData *capsule) const;
      virtual void SphereToCapsule(Contacts &contacts, ColliderData *sphere,
        ColliderData *capsule) const;
      virtual void CapsuleToCapsule(Contacts &contacts, ColliderData *colliderA,
        ColliderData *colliderB) const;
    private:

      struct Projection
      {
        Projection(Real min = SF_REAL_MAX, Real max = -SF_REAL_MAX) : m_min(min), m_max(max) {}
        Real m_min;
        Real m_max;
      };

      bool _FindSeparatingAxis(const std::vector<Vector3> &worldVertsA, 
        const std::vector<Vector3> &worldVertsB, const ColliderGeometry &colGeomA,
        const ColliderGeometry &colGeomB, const Vector3 &posA, const Vector3 &posB, 
        const Quaternion &orientA, const Quaternion &orientB, Vector3 &penetAxis, Real &penetration) const;

      bool _IsAxisSeparating(const std::vector<Vector3> &worldVertsA,
        const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
        const Quaternion &orientA, const Quaternion &orientB, const Vector3 &axis, 
        Real &penetration) const;

      Projection _ProjectOnAxis(const std::vector<Vector3> &worldVerts,  
        const Vector3 &pos, const Quaternion &orient, const Vector3 &axis) const;

      bool _IsOverlaping(const Projection &p1, const Projection &p2) const;

      Real _CalculatePenetration(const Projection &p1, const Projection &p2) const;

      std::vector<Vector3> _GenerateContact(const ColliderGeometry &colGeomA,
        const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA, 
        const std::vector<Vector3> &worldVertsB, const Vector3 &posA, 
        const Vector3 &posB, const Quaternion &orientA, const Quaternion &orientB, 
        const Vector3 &contactNormal) const;

      void _ClipPointsToPlane(std::vector<Vector3> &verts, const Vector3 &plane, Real planeW) const;
    };*/

    namespace SAT
    {
      struct Projection
      {
        Projection(Real min = SF_REAL_MAX, Real max = -SF_REAL_MAX) : m_min(min), m_max(max) {}
        Real m_min;
        Real m_max;
      };

      struct FaceQuery
      {
        int m_index;
        Real m_separation;
      };

      struct EdgeQuery
      {
        int m_edgeA;
        int m_edgeB;
        Real m_separation;
      };

      Projection ProjectOnAxis(const std::vector<Vector3> &worldVerts, Vector3 &axis);
      Real CalculatePenetration(const Projection &p1, const Projection &p2);
      void ClipPointsToPlane(std::vector<Vector3> &verts, const Geometry::Plane &plane);

      //Returns false if both points are outside
      bool ClipLineToPlane(Vector3 &p0, Vector3 &p1, const Vector3 &plane, Real planeD);

      void QueryFaceDirections(const Vector3 &posA, const Vector3 &scaleA, const Quaternion &orientA,
        const ColliderGeometry &hullA, const Vector3 &posB, const Vector3 &scaleB,
        const Quaternion &orientB, const ColliderGeometry &hullB, FaceQuery &faceQuery);
      void QueryEdgeDirections(const Vector3 &posA, const Vector3 &scaleA, const Quaternion &orientA,
        const ColliderGeometry &hullA, const Vector3 &posB, const Vector3 &scaleB,
        const Quaternion &orientB, const ColliderGeometry &hullB, EdgeQuery &edgeQuery);

      Real ProjectPlaneOnHull(const Geometry::Plane &plane, const ColliderGeometry &hull);
      Real ProjectEdge(const Vector3 &pa, const Vector3 &ea, const Vector3 &pb,
        const Vector3 &eb, const Vector3 &posA);

      bool IsMinkowskiFace(const Vector3 &a, const Vector3 &b, const Vector3 &BxA,
        const Vector3 &c, const Vector3 &d, const Vector3 &DxC);
    }
  }
}