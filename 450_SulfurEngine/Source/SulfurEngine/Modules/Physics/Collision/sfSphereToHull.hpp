/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereToHull.hpp
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

    void SphereToHull(Contacts &contacts, ColliderData *sphere,
      ColliderData *hull);

    inline void HullToSphere(Contacts &contacts, ColliderData *hull,
      ColliderData *sphere)
    {
      SphereToHull(contacts, sphere, hull);
    }
  }
}