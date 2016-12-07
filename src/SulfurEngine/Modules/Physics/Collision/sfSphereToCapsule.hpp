#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void SphereToCapsule(Contacts &contacts, ColliderData *sphere,
      ColliderData *capsule);

    inline void CapsuleToSphere(Contacts &contacts, ColliderData *capsule,
      ColliderData *sphere)
    {
      SphereToCapsule(contacts, sphere, capsule);
    }
  }
}