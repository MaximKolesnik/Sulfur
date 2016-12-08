/******************************************************************************/
/*!
\par     Sulfur
\file    sfEvents.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "sfOnTestEvent.hpp"

namespace Sulfur
{
#define SF_OBJECT_EVENT                                                 \
private:                                                                \
  friend class EventManager;                                            \
  template <typename EventData, typename EventFunc>     \
  void _SendEventsToComps(EventFunc func, EventData *data)                              \
  {                                                                     \
    for (auto &it : m_components)                                       \
    {                                                                   \
      IEntity *comp = ComponentFactory::Instance()                      \
        ->GetComponent(it.first, it.second);                            \
      (comp->*func)(data);                                                 \
    }                                                                   \
  }

#define SF_COMPONENT_EVENTS                                             \
public:                                                                 \
  friend class EventManager;                                            \
  virtual void OnTestEvent(OnTestEventData *eventData) {}  
}