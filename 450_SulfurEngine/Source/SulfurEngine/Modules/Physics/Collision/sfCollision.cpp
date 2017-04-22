/******************************************************************************/
/*!
\par     Sulfur
\file    sfCollision.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfCollision.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"

#include "sfHullToHull.hpp"
#include "sfHullToCapsule.hpp"
#include "sfCapsuleToCapsule.hpp"
#include "sfSphereToHull.hpp"
#include "sfSphereToCapsule.hpp"
#include "sfSphereToSphere.hpp"

namespace Sulfur
{
  namespace Physics
  {
    typedef void (*CollisionFunc)(Contacts &, ColliderData *, ColliderData *);

    static const CollisionFunc 
      collisionFuncs[ColliderType::CT_NUM_TYPES][ColliderType::CT_NUM_TYPES] =
    {
      SphereToSphere, SphereToCapsule, SphereToHull,
      CapsuleToSphere, CapsuleToCapsule, CapsuleToHull,
      HullToSphere, HullToCapsule, HullToHull
    };

    void Collide(Contacts &contacts,
      ColliderData *colliderA, ColliderData *colliderB)
    {
      if (colliderA->m_rbData->m_state == RB_Static
        && colliderB->m_rbData->m_state == RB_Static)
        return;

      collisionFuncs[colliderA->m_type][colliderB->m_type](contacts, colliderA, colliderB);
    }
  }
}