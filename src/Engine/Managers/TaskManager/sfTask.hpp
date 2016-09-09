/******************************************************************************/
/*!
\par     Sulfur
\file    sfTask.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Task

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <functional>

#include "sfWorker.hpp"
#include "../../Settings/EngineSettings.h"

namespace Sulfur
{
  typedef void(*MainTaskPtr)(void);

  VOID CALLBACK FiberRoutine(PVOID lpParam);

  class ITask
  {
  public:
    ITask(const std::string &taskName) : m_name(taskName), m_fiber(NULL) {};
    virtual ~ITask(void)
    {
      DeleteFiber(m_fiber);
    }

    virtual void Execute(void) = 0;

    const std::string GetName(void) const { return m_name; }

    std::string m_name;
    Fiber m_fiber;
    WorkerThread *m_worker; //set by worker
  };

  template <class... Args>
  class Task : public ITask //Used as a subTask
  {
    typedef void(*FuncPtr)(Args...);
  public:
    Task(FuncPtr funcPtr, Args... args) 
      : ITask("__SubTask")
    {
      auto binder = std::bind(funcPtr, std::forward<Args...>(args)...);
      m_boundFunc = [binder] {binder();};

      m_fiber = CreateFiberEx(EngineSettings::FiberStackSize,
        EngineSettings::FiberReservedStackSize, FIBER_FLAG_FLOAT_SWITCH,
        FiberRoutine, this);
    }

    virtual ~Task(void) {};

    virtual void Execute(void) override
    {
      m_boundFunc();
    }

  private:
    std::function<void()> m_boundFunc;
  };

  template <>
  class Task<void> : public ITask
  {
  public:
    Task(const std::string &name, MainTaskPtr funcPtr)
      : ITask(name), m_funcPtr(funcPtr)
    {
      m_fiber = CreateFiberEx(EngineSettings::FiberStackSize,
        EngineSettings::FiberReservedStackSize, FIBER_FLAG_FLOAT_SWITCH,
        FiberRoutine, this);
    }

    virtual void Execute(void) override
    {
      m_funcPtr();
    }

  private:
    MainTaskPtr m_funcPtr;
  };
}