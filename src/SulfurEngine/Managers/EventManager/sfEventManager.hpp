#pragma once

#include <queue>
#include <mutex>

#include "Types\sfSingleton.hpp"
#include "Events\sfIEventData.hpp"
#include "Managers\TaskManager\sfTaskRegistry.hpp"
#include "Factories\sfObjectFactory.hpp"

namespace Sulfur
{
  class IEntity;
  class EventManager
  {
    SF_SINGLETON(EventManager);
  public:
    //template <void (IEntity::*EventFunc) (typename EventDataType *)>
    //void PushEvent(const EventDataType &data)
    //{
    //  m_queueMutex.lock();   //Queue lock

    //  m_eventQueue.push(new EventMessage<EventFunc>(data));

    //  m_queueMutex.unlock();
    //}

    //Generally should be used only from task manager
    void TriggerAll(void);

  private:

    struct IEventMessage
    {
      virtual void Trigger(void) = 0;
    };

    template <void (IEntity::*EventFunc) (int *) >
    struct EventMessage : public IEventMessage
    {
      EventMessage(const int &data)
        : m_data(data)
      {

      }

      virtual void Trigger(void) override
      {
        Object *obj = SF_GET_OBJECT(m_data.m_thisObject);
        obj->_SendEventsToComps<EventFunc>(&m_data);
      }

      int  m_data;
    };

    std::mutex   m_queueMutex;
    std::queue<IEventMessage*> m_eventQueue;
  };

  SF_DECLARE_TASK(TriggerEventsEndFrame);
}