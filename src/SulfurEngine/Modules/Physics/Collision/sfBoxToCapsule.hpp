#pragma once

#include "sfContact.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    void BoxToCapsule(Contacts &contacts, ColliderData *box,
      ColliderData *capsule);

    inline void CapsuleToBox(Contacts &contacts, ColliderData *capsule,
      ColliderData *box)
    {
      BoxToCapsule(contacts, box, capsule);
    }
  }
}