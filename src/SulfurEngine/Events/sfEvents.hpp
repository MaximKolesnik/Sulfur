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