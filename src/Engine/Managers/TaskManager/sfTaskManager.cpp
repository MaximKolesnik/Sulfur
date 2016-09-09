/******************************************************************************/
/*!
\par     Sulfur
\file    sfTaskManager.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Task manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfTaskManager.hpp"
#include "sfWorker.hpp"
#include "../../Error/sfError.hpp"
#include "sfTaskRegistry.hpp"
#include "sfDependencyGraph.hpp"
#include "../../Settings/EngineSettings.h"

namespace Sulfur
{
  TaskManager* TaskManager::Instance(void)
  {
    static TaskManager instance;

    return &instance;
  }

  void TaskManager::RunTasks(void)
  {
    m_depGraph->Restart();

    for (UINT32 i = 1; i < m_numThreads; ++i)
      ResumeThread(m_workers[i].m_threadHandle);

    WorkerThreadRoutine(&m_workers[0]);

    for (UINT32 i = 1; i < m_numThreads; ++i)
      SuspendThread(m_workers[i].m_threadHandle);
  }

  void TaskManager::AddIndependentNode(const std::string &taskName)
  {
    m_depGraph->AddIndependentNode(taskName);
  }

  void TaskManager::AddDependentNode(const std::string &taskName,
    const std::string &dependencyName)
  {
    m_depGraph->AddDependentNode(taskName, dependencyName);
  }

  void TaskManager::CompleteGraph(void)
  {
    m_depGraph->CompleteGraph();
  }

  bool TaskManager::IsDone(void) const
  {
    return m_depGraph->AreAllTasksDone();
  }

  ITask* TaskManager::PullTask(void)
  {
    
    if (m_taskQueue.empty())
    {
      std::string taskName;

      m_graphMutex.lock();  //Lock graph
      if (m_depGraph->TryPullTask(taskName))
      { 
        SF_ASSERT(m_mainTaskRegistry.find(taskName) != m_mainTaskRegistry.end(),
          "Graph returned unregistered task");

        m_graphMutex.unlock(); //Unlock graph
        return m_mainTaskRegistry[taskName];
      }
      m_graphMutex.unlock(); //Unlock graph

      return nullptr;
    }
    else
    {
      m_queueMutex.lock(); //Lock taskQueue

      ITask *task = m_taskQueue.front();
      m_taskQueue.pop_front();

      m_queueMutex.unlock(); //Unlock taskQueue

      return task;
    }
  }

  TaskManager::TaskManager(void) : m_workers(nullptr), m_destroy(false)
  {
    m_numThreads = std::thread::hardware_concurrency();
    SF_CRITICAL_ERR_EXP(m_numThreads != 0, "Number of concurent threads is not computable");
    
    _CreateWorkerThreads();
    
    //Fill internal registry
    TaskRegistry::REGMAP &regMap = TaskRegistry::GetRegistry();
    for (auto &it : regMap)
    {
      std::pair<std::string, ITask*> newPair;
      newPair.first = it.first;
      newPair.second = new Task<void>(it.first, it.second);

      SF_ASSERT(m_mainTaskRegistry.find(it.first) == m_mainTaskRegistry.end(),
        std::string("Task " + it.first + " is already registered").c_str());

      m_mainTaskRegistry.emplace(newPair);
    }

    m_depGraph = new DependencyGraph();
  }

  TaskManager::~TaskManager(void)
  {
   /* SF_ASSERT(m_workersWaiting.size() == 0, "There are workers waiting");

    m_destroy = true;
    m_wakeUpCondition.notify_all();

    for (UINT32 i = 0; i < m_numThreads; ++i)
      m_activeWorkers[i]->join();

    for (auto &it : m_activeWorkers)
      delete it;

    while (!m_freeWorkers.empty())
    {
      delete m_freeWorkers.top();
      m_freeWorkers.pop();
    }*/
  }

  void TaskManager::_ProcessCompletedTask(ITask *task)
  {
    std::string taskName = task->GetName();
    if (m_mainTaskRegistry.find(taskName) == m_mainTaskRegistry.end())
      delete task;
    else
    {
      m_graphMutex.lock();
      m_depGraph->NotifyTaskCompletion(taskName);
      m_graphMutex.unlock();
    }
  }

  void TaskManager::_CreateWorkerThreads(void)
  {
    SF_ASSERT(m_workers == nullptr, "Workers are already created");
    m_workers = new WorkerThread[m_numThreads];

    m_workers[0].m_threadHandle = GetCurrentThread();
    SF_CRITICAL_ERR_EXP(m_workers[0].m_threadHandle != NULL, 
      std::to_string(GetLastError()));

    SetThreadAffinityMask(m_workers[0].m_threadHandle, 1);
    m_workers[0].m_coreAffinity = 0;
    m_workers[0].m_taskManager = this;

    for (UINT32 i = 1; i < m_numThreads; ++i)
    {
      m_workers[i].m_threadHandle = CreateThread(0, 0, WorkerThreadRoutine, 
        &m_workers[i], CREATE_SUSPENDED, NULL);

      SF_CRITICAL_ERR_EXP(m_workers[i].m_threadHandle != NULL,
        std::to_string(GetLastError()));

      SetThreadAffinityMask(m_workers[i].m_threadHandle, 1i64 << i);
      m_workers[i].m_coreAffinity = 1;
      m_workers[i].m_taskManager = this;
    }
  }
}