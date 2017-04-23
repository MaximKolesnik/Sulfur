/******************************************************************************/
/*!
\par     Sulfur
\file    sfOnTestEvent.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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