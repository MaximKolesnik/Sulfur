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

namespace Sulfur
{
  typedef void(*MainTaskPtr)(void);

  class ITask
  {
  public:
    ITask() {};
    virtual void operator()(void) = 0;
  };

  template <class... Args>
  class Task : public ITask
  {
    typedef void(*FuncPtr)(Args...);
  public:
    Task(FuncPtr funcPtr, Args... args)
    {
      auto binder = std::bind(funcPtr, std::forward<Args...>(args)...);
      m_boundFunc = [binder] {binder();};
    }

    virtual void operator()(void) override
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
    Task(MainTaskPtr funcPtr) : m_funcPtr(funcPtr)
    {
      
    }

    virtual void operator()(void) override
    {
      m_funcPtr();
    }

  private:
    MainTaskPtr m_funcPtr;
  };
}