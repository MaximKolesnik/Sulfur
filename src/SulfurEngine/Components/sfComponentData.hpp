/******************************************************************************/
/*!
\par     Sulfur
\file    sfComponentData.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Component Factory registers all components in Initialize()

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "DataStructures\sfSlotMap.hpp"

namespace Sulfur
{
  class ComponentData
  {
  public:
    ComponentData(ISlotMap )
  private:
    ISlotMap *m_slotMap;
  };
}