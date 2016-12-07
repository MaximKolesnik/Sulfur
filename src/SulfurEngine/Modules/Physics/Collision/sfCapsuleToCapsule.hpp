#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void CapsuleToCapsule(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB);
  }
}