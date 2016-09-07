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
}

#define DECLARE_TASK(TaskName)                                \
void TaskName(void);                                          \
namespace TaskRegistry                                        \
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
}                                                             

#define DEFINE_TASK(TaskName) \
void TaskName(void)