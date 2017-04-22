/******************************************************************************/
/*!
\par     Sulfur
\file    sfIContactGenerator.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Math\sfVector3.hpp"
#include "Modules\Physics\Collision\sfContact.hpp"

namespace Sulfur
{
  class QueryResults;

  namespace Physics
  {
    class IContactGenerator
    {
    public:
      typedef void (IContactGenerator::*CollisionFunc)(Contacts &, 
        ColliderData *, ColliderData *) const;

      IContactGenerator(void) {}
      virtual ~IContactGenerator(void) {}

      virtual void BoxToBox(Contacts &contacts,
        ColliderData *colliderA, ColliderData *colliderB) const = 0;
      virtual void BoxToSphere(Contacts &contacts, ColliderData *box,
        ColliderData *sphere) const
      {
        SphereToBox(contacts, sphere, box);
      }
      virtual void BoxToCapsule(Contacts &contacts, ColliderData *box,
        ColliderData *capsule) const = 0;

      virtual void SphereToSphere(Contacts &contacts, ColliderData *colliderA,
        ColliderData *colliderB) const = 0;
      virtual void SphereToBox(Contacts &contacts, ColliderData *sphere, 
        ColliderData *box) const = 0;
      virtual void SphereToCapsule(Contacts &contacts, ColliderData *sphere,
        ColliderData *capsule) const = 0;

      virtual void CapsuleToCapsule(Contacts &contacts, ColliderData *colliderA,
        ColliderData *colliderB) const = 0;
      virtual void CapsuleToBox(Contacts &contacts, ColliderData *capsule,
        ColliderData *box) const
      {
        BoxToCapsule(contacts, box, capsule);
      }
      virtual void CapsuleToSphere(Contacts &contacts, ColliderData *capsule,
        ColliderData *sphere) const
      {
        SphereToCapsule(contacts, sphere, capsule);
      }
    private:
    };
  }
}