/******************************************************************************/
/*!
\par     Sulfur
\file    sfIEventData.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfTypes.hpp"

namespace Sulfur
{
  struct IEventData
  {
    IEventData(HNDL thisObject) : m_thisObject(thisObject) {}

    HNDL m_thisObject;
  };
}