/******************************************************************************/
/*!
\par     Sulfur
\file    sfEventManager.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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