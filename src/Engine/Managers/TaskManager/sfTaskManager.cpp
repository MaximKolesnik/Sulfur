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

namespace Sulfur
{
  TaskManager* TaskManager::m_instance = nullptr;

  TaskManager* TaskManager::Instance(void)
  {
    if (!m_instance)
      m_instance = new TaskManager();

    return m_instance;
  }

  void TaskManager::RunTasks(void)
  {
    m_stop = true;
    m_stopCondition.notify_all();
  }

  TaskManager::TaskManager(void) : m_stop(true)
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
      newPair.second = new Task<void>(it.second);

      SF_ASSERT(m_mainTaskRegistry.find(it.first) == m_mainTaskRegistry.end(),
        std::string("Task " + it.first + " is already registered").c_str());

      m_mainTaskRegistry.emplace(newPair);
    }
  }

  TaskManager::~TaskManager(void)
  {
    SF_ASSERT(m_workersWaiting.size() == 0, "There are workers waiting");

    m_stop = true;
    m_stopCondition.notify_all();

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

}