/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToBox.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSphereToBox.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"
#include "Math\Geometry\sfGeometry.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void SphereToBox(Contacts &contacts, ColliderData *sphere,
      ColliderData *box)
    {
      Transform *transSphere = SF_GET_COMP_TYPE(Transform, sphere->m_transformHndl);
      Transform *transBox = SF_GET_COMP_TYPE(Transform, box->m_transformHndl);

      SF_ASSERT(transSphere, "Transform handle on sphere is not set");
      SF_ASSERT(transBox, "Transform handle on box B is not set");

      Vector3 spherePos = transSphere->GetTranslation() + sphere->m_offset;

      Vector3 halfSizes;
      Vector3 boxPos;

      halfSizes = (transBox->GetScale() * box->m_scale) / 2;
      boxPos = transBox->GetTranslation() + box->m_offset;

      Real radius = transSphere->GetScale().MaxAxisValue() * sphere->m_radius;
      Vector3 relSpherePos = transBox->GetRotation().Inverted().Rotated(spherePos - boxPos);

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
        boxMat.SetTransformation(transBox->GetRotation(), transBox->GetScale() * box->m_scale,
          boxPos);

        const ColliderGeometry &boxGeometry = GeometryMap::Instance()->GetBoxGeometry();

        for (auto &p : boxGeometry.GetPlanes())
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

        c.m_colliderA = box;
        c.m_colliderB = sphere;
        c.m_contactPoint = minProj;
        c.m_contactNormal = minNormal;
        c.m_penetration = minDist;
        contacts.push_back(c);
      }
      else //Shallow
      {
        Vector3 closestPointWorld = boxPos + transBox->GetRotation().Rotated(closestPoint);

        c.m_colliderA = box;
        c.m_colliderB = sphere;
        c.m_contactNormal = (spherePos - closestPointWorld).Normalized();
        c.m_contactPoint = closestPointWorld;
        c.m_penetration = radius - MathUtils::Sqrt(dist);
        contacts.push_back(c);
      }
    }
  }
}