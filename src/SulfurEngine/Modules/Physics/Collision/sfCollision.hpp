#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void Collide(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB);
  }
}