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