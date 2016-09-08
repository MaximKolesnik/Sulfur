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
  class DependencyGraph;

  class TaskManager
  {
  public:
    static TaskManager* Instance(void);

    void RunTasks(void);

    void AddIndependentNode(const std::string &taskName);
    void AddDependentNode(const std::string &taskName, const std::string &dependencyName);
    void CompleteGraph(void);

    template <class... Args>
    void EnqueueSubTask(void(*funcPtr)(Args...), std::condition_variable_any &cv,
      Args... args)
    {
      m_queueMutex.lock();

      m_taskQueue.push_back(new Task<Args...>(funcPtr, cv, std::forward<Args...>(args)...));
      m_wakeUpCondition.notify_one();

      m_queueMutex.unlock();
    }

  private:
    //static TaskManager *m_instance;

    friend class Worker;

    TaskManager(void);
    ~TaskManager(void);

    TaskManager(const TaskManager &) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    void _ProcessCompletedTask(ITask *task);

    UINT32 m_numThreads; //hardware concurrent threads

    std::vector<std::thread*> m_activeWorkers;

    std::unordered_map<std::thread::id, std::thread*> m_workersWaiting;
    std::stack<std::thread*> m_freeWorkers;

    std::deque<ITask*> m_taskQueue;
    std::mutex m_queueMutex;
    std::mutex m_graphMutex;

    std::condition_variable m_wakeUpCondition;
    bool m_destroy;

    std::unordered_map<std::string, ITask*> m_mainTaskRegistry;

    DependencyGraph *m_depGraph;
  };

#define SF_ENQUEUE_SUBTASK(FuncPtr, ...) \
++_local_SubTaskCounter;                                 \
Sulfur::TaskManager::Instance()->EnqueueSubTask(FuncPtr, \
  _local_SubTaskCounterCV, __VA_ARGS__)

}
