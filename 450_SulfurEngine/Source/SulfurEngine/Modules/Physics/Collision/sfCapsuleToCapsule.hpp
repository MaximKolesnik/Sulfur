/******************************************************************************/
/*!
\par     Sulfur
\file    sfCapsuleToCapsule.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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