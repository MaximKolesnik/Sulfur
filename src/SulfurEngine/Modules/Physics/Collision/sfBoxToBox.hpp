#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void BoxToBox(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB);
  }
}
