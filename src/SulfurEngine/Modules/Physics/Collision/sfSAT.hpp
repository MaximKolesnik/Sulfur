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
    class SAT : public IContactGenerator
    {
    public:
      SAT(void) : IContactGenerator() {}
      virtual ~SAT(void) {}

      virtual void BoxToBox(Contacts &contacts, 
        ColliderData *colliderA, ColliderData *colliderB) const override;
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

      std::vector<ContactPoint> _GenerateContact(const ColliderGeometry &colGeomA, 
        const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA, 
        const std::vector<Vector3> &worldVertsB, const Vector3 &posA, 
        const Vector3 &posB, const Quaternion &orientA, const Quaternion &orientB, 
        const Vector3 &contactNormal) const;

      void _ClipConHullToConHull(const ColliderGeometry &colGeomA,
        const ColliderGeometry &colGeomB, const std::vector<Vector3> &worldVertsA,
        const std::vector<Vector3> &worldVertsB, const Vector3 &posA, const Vector3 &posB, 
        const Quaternion &orientA, const Quaternion &orientB, const Vector3 &contactNormal, 
        std::vector<ContactPoint> &contactPoints) const;

      void _ClipPointsToPlane(std::vector<Vector3> &verts, const Vector3 &plane, Real planeW) const;
    };
  }
}