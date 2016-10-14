#pragma once

#include "Managers\EventManager\sfEventManager.hpp"
#include "sfOnTestEvent.hpp"

namespace Sulfur
{
#define SF_OBJECT_EVENT                                                 \
private:                                                                \
  friend class EventManager;                                            \
  template <void (IEntity::*Func) (typename EventDataType*)>            \
  void _SendEventsToComps(EventDataType *data)                          \
  {                                                                     \
    for (auto &it : m_components)                                       \
    {                                                                   \
      IEntity *comp = ComponentFactory::Instance()                      \
        ->GetComponent(it.first, it.second);                            \
      comp->Func(data);                                                 \
    }                                                                   \
  }

#define SF_COMPONENT_EVENTS                                             \
public:                                                                 \
  friend class EventManager;                                            \
  virtual void OnTestEvent(OnTestEventData *eventData) {}  
}