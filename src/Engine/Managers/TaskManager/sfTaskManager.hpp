/******************************************************************************/
/*!
\par     Sulfur
\file    sfTaskManager.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Task manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <stack>
#include <unordered_map>
#include <deque>

#include "../../Types/sfTypes.hpp"
#include "sfTask.hpp"

namespace Sulfur
{
  class Worker;

  class TaskManager
  {
  public:
    static TaskManager* Instance(void);

    void RunTasks(void);

    template <class... Args>
    void EnqueueSubTask(void(*funcPtr)(Args...), Args... args)
    {
      m_taskQueue.push_back(new Task<Args...>(funcPtr, std::forward<Args...>(args)...));
    }

  private:
    static TaskManager *m_instance;

    friend class Worker;

    TaskManager(void);
    ~TaskManager(void);

    TaskManager(const TaskManager &) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    UINT32 m_numThreads; //hardware concurrent threads
    
    std::vector<std::thread*> m_activeWorkers;

    std::unordered_map<std::thread::id, std::thread*> m_workersWaiting;
    std::stack<std::thread*> m_freeWorkers;

    std::deque<ITask*> m_taskQueue;
    std::mutex m_queueMutex;

    std::condition_variable m_stopCondition; //suspend threads if there is nothing to do
    bool m_stop; //If tasks are currently being processed

    std::unordered_map<std::string, ITask*> m_mainTaskRegistry;
  };

// Job name must be unique
#define SF_DECLARE_TASK(FuncName) \
Sulfur::TaskManager::Instance()->RegisterTask(#FuncName, FuncName) 

}