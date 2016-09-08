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
    typedef std::unordered_map<std::string, MainTaskPtr> REGMAP;

    inline REGMAP& GetRegistry(void)
    {
      static REGMAP reg;
      return reg;
    }

    template <MainTaskPtr>
    class Entry
    {
    public:
      Entry(const std::string &name, MainTaskPtr funcPtr)
      {
        REGMAP &reg = GetRegistry();
        reg.emplace(name, funcPtr);
      }
    };

    template <MainTaskPtr>
    class TaskRegistration;
  }

#define SF_DECLARE_TASK(TaskName)                             \
void TaskName(void);                                          \
namespace TaskRegistry { namespace                            \
{                                                             \
template <MainTaskPtr>                                        \
class TaskRegistration;                                       \
  template <>                                                 \
  class TaskRegistration<TaskName>                            \
  {                                                           \
    static const Entry<TaskName> &entry;                      \
  };                                                          \
    const Entry<TaskName> &TaskRegistration<TaskName>::entry  \
    = Entry<TaskName>(std::string(#TaskName), TaskName);      \
}     }                                                         

#define SF_DEFINE_TASK(TaskName)                    \
void TaskName(void)                                 \
{                                                   \
  UINT32 _local_SubTaskCounter = 0;                 \
  std::condition_variable_any _local_SubTaskCounterCV;  

#define SF_END_DEFINE_TASK(TaskName) }       

#define SF_DEFINE_SUBTASK(TaskName, ...)                \
void TaskName(__VA_ARGS__)                               \
{                                                       \
UINT32 _local_SubTaskCounter = 0;                       \
std::condition_variable_any _local_SubTaskCounterCV;        

#define SF_END_DEFINE_SUBTASK(TaskName) }

#define SF_WAIT_FOR_SUBTASKS()                              \
static std::recursive_mutex _static_mutex;                            \
static std::unique_lock<std::recursive_mutex> lock(_static_mutex);    \
while (_local_SubTaskCounter)                              \
{                                                           \
  _local_SubTaskCounterCV.wait(lock);                        \
  --_local_SubTaskCounter;                                  \
}

}