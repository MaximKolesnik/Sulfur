/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToHull.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSphereToHull.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Math\Geometry\sfGeometry.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void SphereToHull(Contacts &contacts, ColliderData *sphere,
      ColliderData *hull)
    {
      Transform *transSphere = SF_GET_COMP_TYPE(Transform, sphere->m_transformHndl);
      Transform *transHull = SF_GET_COMP_TYPE(Transform, hull->m_transformHndl);

      SF_ASSERT(transSphere, "Transform handle on sphere is not set");
      SF_ASSERT(transHull, "Transform handle on box B is not set");

      Vector3 spherePos = transSphere->GetTranslation() + sphere->m_offset;

      Vector3 halfSizes;
      Vector3 boxPos;

      halfSizes = (transHull->GetScale() * hull->m_scale) / 2;
      boxPos = transHull->GetTranslation() + hull->m_offset;

      Real radius = transSphere->GetScale().MaxAxisValue() * sphere->m_radius;
      Vector3 relSpherePos = transHull->GetRotation().Inverted().Rotated(spherePos - boxPos);

      if (MathUtils::Abs(relSpherePos[0]) - radius > halfSizes[0]
        ||
        MathUtils::Abs(relSpherePos[1]) - radius > halfSizes[1]
        ||
        MathUtils::Abs(relSpherePos[2]) - radius > halfSizes[2])
        return;

      Vector3 closestPoint(0, 0, 0);
      Real dist;

      for (int i = 0; i < 3; ++i)
      {
        dist = relSpherePos[i];

        if (dist > halfSizes[i])
          dist = halfSizes[i];
        if (dist < -halfSizes[i])
          dist = -halfSizes[i];

        closestPoint[i] = dist;
      }

      dist = (closestPoint - relSpherePos).LengthSq();
      if (dist > radius * radius)
        return;

      Contact c;

      if (closestPoint == relSpherePos) // Deep contact
      {
        Real minDist = SF_REAL_MAX;
        Vector3 minNormal;
        Vector3 minProj;
        Matrix4 boxMat;
        boxMat.SetTransformation(transHull->GetRotation(), transHull->GetScale() * hull->m_scale,
          boxPos);

        const ColliderGeometry *hullGeometry = hull->m_geometry;

        for (auto &p : hullGeometry->GetPlanes())
        {
          Geometry::Plane plane = p.Transformed(boxMat);

          Vector3 proj = Geometry::ProjectPointOnPlane(spherePos, plane.GetNormal(), plane.GetDistance());
          Real dist = (spherePos - proj).Length();
          if (dist < minDist)
          {
            minDist = dist;
            minNormal = plane.GetNormal();
            minProj = proj;
          }
        }

        c.m_colliderA = hull;
        c.m_colliderB = sphere;
        c.m_contactPoint = minProj;
        c.m_contactNormal = minNormal;
        c.m_penetration = minDist;
        contacts.push_back(c);
      }
      else //Shallow
      {
        Vector3 closestPointWorld = boxPos + transHull->GetRotation().Rotated(closestPoint);

        c.m_colliderA = hull;
        c.m_colliderB = sphere;
        c.m_contactNormal = (spherePos - closestPointWorld).Normalized();
        c.m_contactPoint = closestPointWorld;
        c.m_penetration = radius - MathUtils::Sqrt(dist);
        contacts.push_back(c);
      }
    }
  }
}