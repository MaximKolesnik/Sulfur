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
    ITask(const std::string &taskName) : m_name(taskName) {};
    virtual void operator()(void) = 0;

    const std::string GetName(void) const { return m_name; }

  protected:
    std::string m_name;
  };

  template <class... Args>
  class Task : public ITask //Used as a subTask
  {
    typedef void(*FuncPtr)(Args...);
  public:
    Task(FuncPtr funcPtr, std::condition_variable_any &cv, Args... args) 
      : ITask("__SubTask"), m_cv(cv)
    {
      auto binder = std::bind(funcPtr, std::forward<Args...>(args)...);
      m_boundFunc = [binder] {binder();};
    }

    virtual void operator()(void) override
    {
      m_boundFunc();
      m_cv.notify_one();
    }

  private:
    std::function<void()> m_boundFunc;
    std::condition_variable_any &m_cv;
  };

  template <>
  class Task<void> : public ITask
  {
  public:
    Task(const std::string &name, MainTaskPtr funcPtr)
      : ITask(name), m_funcPtr(funcPtr)
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