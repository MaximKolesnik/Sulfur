/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToCapsule.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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