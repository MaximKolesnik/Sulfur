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

#include <atomic>
#include <string>

#include "sfWorker.hpp"

namespace Sulfur
{
  struct Task;
  class TaskManager;

  typedef VOID (CALLBACK *TaskPtr)(PVOID lpParam);
  
  struct Task
  {
    struct Counter
    {
      std::atomic<INT32> m_counter = 0;
      Task *m_waitingTask = nullptr;
    };

    Task(const std::string &name, TaskPtr funcPtr) : m_funcPtr(funcPtr), 
      m_taskName(name) 
    {
      m_waitingCounter.m_waitingTask = this;
    }

    const TaskPtr m_funcPtr;

    Counter m_waitingCounter; //this task waking counter
    Counter *m_waitingTaskCounter = nullptr; //counter of a task that is waiting on this task

    Fiber m_fiber = NULL;

    std::string m_taskName = "_NotAssigned";

    WorkerThread *m_executingWorker = nullptr;

    bool m_done = false;
    std::atomic<bool> m_waiting = false;

    void *m_data = nullptr;

    TaskManager *m_taskManager = nullptr;
  };
}