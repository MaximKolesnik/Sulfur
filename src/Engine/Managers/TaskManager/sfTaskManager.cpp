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

namespace Sulfur
{
  //TaskManager* TaskManager::m_instance = nullptr;

  TaskManager* TaskManager::Instance(void)
  {
    static TaskManager instance;

    return &instance;
  }

  void TaskManager::RunTasks(void)
  {
    m_depGraph->Restart();

    std::string pulledTask;
    while (!m_depGraph->AreAllTasksDone())
    {
      if (m_depGraph->TryPullTask(pulledTask))
      {
        m_taskQueue.push_back(m_mainTaskRegistry[pulledTask]);
        m_wakeUpCondition.notify_one();
      }
    }
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

  TaskManager::TaskManager(void) : m_destroy(false)
  {
    m_numThreads = std::thread::hardware_concurrency();
    SF_CRITICAL_ERR_EXP(m_numThreads != 0, "Number of concurent threads is not computable");

    for (UINT32 i = 0; i < m_numThreads; ++i)
      m_activeWorkers.push_back(new std::thread(Worker()));

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
    SF_ASSERT(m_workersWaiting.size() == 0, "There are workers waiting");

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
    }
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
}