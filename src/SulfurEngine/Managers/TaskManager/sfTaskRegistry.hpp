/******************************************************************************/
/*!
\par     Sulfur
\file    sfTaskRegistry.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/6/2016

\brief   Task registry

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>
#include <string>
#include <condition_variable>

#include "sfTask.hpp"

namespace Sulfur
{
  namespace TaskRegistry
  {
    typedef std::unordered_map<std::string, TaskPtr> REGMAP;

    inline REGMAP& GetRegistry(void)
    {
      static REGMAP reg;
      return reg;
    }

    template <TaskPtr>
    class Entry
    {
    public:
      Entry(const std::string &name, TaskPtr funcPtr)
      {
        REGMAP &reg = GetRegistry();
        reg.emplace(name, funcPtr);
      }
    };

    template <TaskPtr>
    class TaskRegistration;
  }


#define SF_UNIQUE_NAME( prefix ) SF_JOIN( prefix, __LINE__ )
#define SF_JOIN( symbol1, symbol2 ) SF_DO_JOIN( symbol1, symbol2 )
#define SF_DO_JOIN( symbol1, symbol2 ) symbol1##symbol2

#define SF_DECLARE_TASK(TaskName)                             \
VOID CALLBACK TaskName(PVOID lpParam);                        \
namespace TaskRegistry { namespace                            \
{                                                             \
template <TaskPtr>                                            \
class TaskRegistration;                                       \
  template <>                                                 \
  class TaskRegistration<TaskName>                            \
  {                                                           \
    static const Entry<TaskName> &entry;                      \
  };                                                          \
    const Entry<TaskName> &TaskRegistration<TaskName>::entry  \
    = Entry<TaskName>(std::string(#TaskName), TaskName);      \
}     }                                                         

#define SF_DEFINE_TASK(TaskName)                                    \
VOID CALLBACK TaskName(PVOID lpParam)                               \
{                                                                   \
  Task *_taskData = reinterpret_cast<Task*>(lpParam);               \
  TaskStart:                                                        \
if (_taskData->m_done)                                              \
    SwitchToFiber(_taskData->m_executingWorker->m_selfFiberHandle); 


#define SF_END_DEFINE_TASK(TaskName)  \
_taskData->m_done = true;             \
 goto TaskStart;}       

#define SF_ENQUEUE_JOB(TaskName, DataPtr)                             \
{                                                                     \
  static int _id = 0;                                                 \
  _taskData->m_executingWorker->m_taskManager->EnqueueTask(TaskName,  \
  (uintptr_t)&_id, DataPtr, #TaskName, _taskData);                    \
}

#define SF_YEILD_AND_WAIT()                                       \
while (_taskData->m_waitingCounter.m_counter != 0)                \
{                                                                 \
  _taskData->m_waiting = true;                                    \
  SwitchToFiber(_taskData->m_executingWorker->m_selfFiberHandle); \
}

#define SF_GET_FIBER_DATA(Type)           \
reinterpret_cast<Type*>(_taskData->m_data)

}