/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToCapsule.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSphereToCapsule.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void SphereToCapsule(Contacts &contacts, ColliderData *sphere,
      ColliderData *capsule)
    {
      Transform *sphereTrans = SF_GET_COMP_TYPE(Transform, sphere->m_transformHndl);
      Transform *capsuleTrans = SF_GET_COMP_TYPE(Transform, capsule->m_transformHndl);

      Vector3 spherePos = sphereTrans->GetTranslation() + sphere->m_offset;
      Vector3 capsulePos = capsuleTrans->GetTranslation() + capsule->m_offset;

      Real capsuleScale = capsuleTrans->GetScale()[0];
      Real cLineLength = capsule->m_lineLength * capsuleScale;
      Real cRadius = capsule->m_radius * std::max(capsuleTrans->GetScale()[1], capsuleTrans->GetScale()[2]);
      Real sRadius = sphere->m_radius * sphereTrans->GetScale().MaxAxisValue();

      //Find closest point to center line
      Vector3 cLineP1 = Vector3(-cLineLength / 2, Real(0.0), Real(0.0));
      Vector3 cLineP2 = Vector3(cLineLength / 2, Real(0.0), Real(0.0));

      capsuleTrans->GetRotation().Rotate(cLineP1);
      capsuleTrans->GetRotation().Rotate(cLineP2);

      cLineP1 += capsulePos;
      cLineP2 += capsulePos;

      Vector3 centerLine = cLineP2 - cLineP1;

      Vector3 closestPoint;
      Real centerLineMag = centerLine.Length();
      Vector3 sphereP1 = spherePos - cLineP1;

      Real projection = centerLine.Dot(sphereP1) / centerLineMag;

      if (projection <= Real(0.0))
        closestPoint = cLineP1;
      else if (projection >= centerLineMag)
        closestPoint = cLineP2;
      else
        closestPoint = cLineP1 * (1 - projection) + cLineP2 * projection;

      Vector3 CtoP = closestPoint - spherePos;
      Real CtoPLength = CtoP.Length();

      if (CtoPLength > cRadius + sRadius)
        return;

      Contact c;

      c.m_colliderA = sphere;
      c.m_colliderB = capsule;
      c.m_contactNormal = CtoP.Normalized();
      c.m_contactPoint = spherePos + CtoP * Real(0.5);
      c.m_penetration = sRadius + cRadius - CtoPLength;

      contacts.push_back(c);
    }
  }
}