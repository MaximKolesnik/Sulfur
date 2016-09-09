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
#include <WinBase.h>

#include "../../Types/sfTypes.hpp"
#include "sfTask.hpp"
#include "sfWorker.hpp"

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
    bool IsDone(void) const;

    ITask* PullTask(void);

    template <class... Args>
    void EnqueueSubTask(void(*funcPtr)(Args...), std::condition_variable_any &cv,
      Args... args)
    {
      m_queueMutex.lock();

      m_taskQueue.push_back(new Task<Args...>(funcPtr, std::forward<Args...>(args)...));
      m_wakeUpCondition.notify_one();

      m_queueMutex.unlock();
    }
    void _ProcessCompletedTask(ITask *task);
  private:
    friend class Worker;

    TaskManager(void);
    ~TaskManager(void);

    TaskManager(const TaskManager &) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    void _CreateWorkerThreads(void);

    UINT32 m_numThreads; //hardware concurrent threads

    WorkerThread *m_workers;

    std::deque<ITask*> m_taskQueue;
    std::mutex m_queueMutex;
    std::mutex m_graphMutex;

    std::condition_variable m_wakeUpCondition;
    bool m_destroy;

    std::unordered_map<std::string, ITask*> m_mainTaskRegistry;

    DependencyGraph *m_depGraph;
  };
}
