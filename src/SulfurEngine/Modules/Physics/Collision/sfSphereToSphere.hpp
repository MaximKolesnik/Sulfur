#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void SphereToSphere(Contacts &contacts,
      ColliderData *colliderA, ColliderData *colliderB);
  }
}