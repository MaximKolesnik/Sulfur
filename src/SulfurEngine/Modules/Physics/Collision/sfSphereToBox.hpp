#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void SphereToBox(Contacts &contacts, ColliderData *sphere,
      ColliderData *box);

    inline void BoxToSphere(Contacts &contacts, ColliderData *box,
      ColliderData *sphere)
    {
      SphereToBox(contacts, sphere, box);
    }
  }
}