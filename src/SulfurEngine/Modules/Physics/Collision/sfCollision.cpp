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

#include "sfBoxToBox.hpp"
#include "sfBoxToCapsule.hpp"
#include "sfCapsuleToCapsule.hpp"
#include "sfSphereToBox.hpp"
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
      BoxToBox, BoxToSphere, BoxToCapsule,
      SphereToBox, SphereToSphere, SphereToCapsule,
      CapsuleToBox, CapsuleToSphere, CapsuleToCapsule
    };

    void Collide(Contacts &contacts,
      ColliderData *colliderA, ColliderData *colliderB)
    {
      collisionFuncs[colliderA->m_type][colliderB->m_type](contacts, colliderA, colliderB);
    }
  }
}