#pragma once

#include <string>

#include "sfIEventData.hpp"

namespace Sulfur
{
  struct OnTestEventData : public IEventData
  {
    OnTestEventData(HNDL thisObj, const std::string &message)
      : IEventData(thisObj), m_message(message) {}

    std::string m_message;
  };
}