#include "sfEventManager.hpp"

namespace Sulfur
{
  SF_DEFINE_TASK(TriggerEventsEndFrame)
  {
    EventManager::Instance()->TriggerAll();
  } SF_END_DEFINE_TASK(TriggerEventsEndFrame);

  EventManager::EventManager(void)
  {

  }

  EventManager::~EventManager(void)
  {

  }

  void EventManager::TriggerAll(void)
  {
    while (!m_eventQueue.empty())
    {
      m_eventQueue.front()->Trigger();
      m_eventQueue.pop();
    }
  }
}