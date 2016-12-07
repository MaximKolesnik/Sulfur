/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToSphere.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSphereToSphere.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void SphereToSphere(Contacts &contacts,
      ColliderData *colliderA, ColliderData *colliderB)
    {
      Vector3 posA, posB;
      Transform *transA = SF_GET_COMP_TYPE(Transform, colliderA->m_transformHndl);
      Transform *transB = SF_GET_COMP_TYPE(Transform, colliderB->m_transformHndl);

      SF_ASSERT(transA, "Transform handle on collider A is not set");
      SF_ASSERT(transB, "Transform handle on collider B is not set");

      posA = transA->GetTranslation() + colliderA->m_offset;
      posB = transB->GetTranslation() + colliderB->m_offset;

      Real radA = transA->GetScale().MaxAxisValue() * colliderA->m_radius;
      Real radB = transB->GetScale().MaxAxisValue() * colliderB->m_radius;
      Vector3 CtoC = posB - posA;
      Real CtoCLength = CtoC.Length();

      if (CtoCLength > radA + radB)
        return;

      Contact c;

      c.m_colliderA = colliderA;
      c.m_colliderB = colliderB;
      c.m_contactNormal = CtoC.Normalized();
      c.m_contactPoint = posA + CtoC * Real(0.5);
      c.m_penetration = radA + radB - CtoC.Length();

      contacts.push_back(c);
    }
  }
}