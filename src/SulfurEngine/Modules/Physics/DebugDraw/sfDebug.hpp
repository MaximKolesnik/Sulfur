/******************************************************************************/
/*!
\par     Sulfur
\file    sfDebug.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void DrawColliders(const PhysicsWorld::ColliderList &colliderList);
    //void DrawContacts()
  }
}