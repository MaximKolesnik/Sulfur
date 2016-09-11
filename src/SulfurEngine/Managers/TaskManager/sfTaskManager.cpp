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
    SF_ASSERT(m_waitingTasks.empty(), "There are not processed waiting tasks");

    m_depGraph->Restart();

    _ResumeThreads();

    WorkerThreadRoutine(&m_workers[0]);
  }

  void TaskManager::AddNode(const std::string &taskName)
  {
    m_depGraph->AddNode(taskName);
  }

  void TaskManager::SetStartingTask(const std::string &taskName)
  {
    m_depGraph->SetStartingTask(taskName);
  }

  void TaskManager::SetDependency(const std::string &taskName,
    const std::string &dependsOn)
  {
    m_depGraph->SetDependency(taskName, dependsOn);
  }

  void TaskManager::CompleteGraph(void)
  {
    m_depGraph->CompleteGraph();
  }

  bool TaskManager::IsDone(void) const
  {
    return m_depGraph->AreAllTasksDone();
  }

  Task* TaskManager::PullTask(WorkerThread *pullingWorker)
  {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    Task *task = nullptr;

    if ((task = _PullAwakeTask(pullingWorker))) //Check for awake task for current thread
      return task;
    else if (m_taskQueue.empty())               //Pull from graph
    {
      std::string taskName;

      m_graphMutex.lock();  //Lock graph
      if (m_depGraph->TryPullTask(taskName))
      { 
        SF_ASSERT(m_taskRegistry.find(taskName) != m_taskRegistry.end(),
          "Graph returned unregistered task");

        m_graphMutex.unlock(); //Unlock graph
        return m_taskRegistry[taskName];
      }
      m_graphMutex.unlock(); //Unlock graph

      return nullptr;
    }
    else
    {
      Task *task = m_taskQueue.front();
      m_taskQueue.pop_front();

      return task;
    }
  }

  TaskManager::TaskManager(void) : m_workers(nullptr)
  {
    m_numThreads = std::thread::hardware_concurrency();
    SF_CRITICAL_ERR_EXP(m_numThreads != 0, "Number of concurent threads is not computable");
    
    _CreateWorkerThreads();
    
    //Fill internal registry
    TaskRegistry::REGMAP &regMap = TaskRegistry::GetRegistry();
    for (auto &it : regMap)
    {
      std::pair<std::string, Task*> newPair;
      newPair.first = it.first;
      
      Task *task = new Task(it.first, it.second);
      task->m_fiber = CreateFiberEx(EngineSettings::FiberStackSize,
        EngineSettings::FiberReservedStackSize, FIBER_FLAG_FLOAT_SWITCH,
        it.second, task);
      task->m_taskManager = this;

      newPair.second = task;

      SF_CRITICAL_ERR_EXP(task->m_fiber != NULL, "Cannot create fiber");

      SF_ASSERT(m_taskRegistry.find(it.first) == m_taskRegistry.end(),
        std::string("Task " + it.first + " is already registered").c_str());

      m_taskRegistry.emplace(newPair);
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

  void TaskManager::_ProcessCompletedTask(Task *task)
  {
    std::string taskName = task->m_taskName;

    if (task->m_waitingTaskCounter)
    {
      SF_ASSERT(task->m_waitingTaskCounter->m_counter.load() > 0, 
        "Waiting counter is invalid");

      --(task->m_waitingTaskCounter->m_counter);

      //Schedule this task if ready
      if (task->m_waitingTaskCounter->m_counter.load() == 0)
      {
        Task *waitingTask = task->m_waitingTaskCounter->m_waitingTask;
        
        SF_ASSERT(waitingTask != nullptr, "Waiting task is not set");

        m_waitingTasksMutex.lock();

        SF_ASSERT(std::find(m_waitingTasks.begin(), m_waitingTasks.end(), 
          waitingTask) != m_waitingTasks.end(), "Waiting task is not on a waiting queue");

        auto it = std::find(m_waitingTasks.begin(), m_waitingTasks.end(),
          waitingTask);
        m_waitingTasks.erase(it);

        m_awakeTasksMutex.lock();
        m_awakeTasks[waitingTask->m_executingWorker->m_threadHandle].push_back(waitingTask);
        m_awakeTasksMutex.unlock();

        m_waitingTasksMutex.unlock();
      }
    }


    m_graphMutex.lock();
    m_depGraph->NotifyTaskCompletion(taskName);
    m_graphMutex.unlock();
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
      InitializeConditionVariable(&m_workers[i].m_suspendedCV);
      InitializeCriticalSection(&m_workers[i].m_suspendedCS);
    }
  }

  void TaskManager::_ProcessWaitingTask(Task *task)
  {
    SF_ASSERT(task->m_done != true, "Task is not waiting");
    SF_ASSERT(task->m_waiting == true, "Task is not waiting");

    m_waitingTasksMutex.lock();     //Lock waiting tasks
    SF_ASSERT(std::find(m_waitingTasks.begin(), m_waitingTasks.end(), task)
      == m_waitingTasks.end(), "Task is already on a waiting queue");

    m_waitingTasks.push_back(task);
    m_waitingTasksMutex.unlock();   //Unlock waiting tasks
  }

  void TaskManager::EnqueueTask(TaskPtr funcPtr, uintptr_t id, void *dataPtr, 
    const std::string &name, Task *parentTask)
  {
    m_dynamicTasksMutex.lock();          //Lock dynamicTasks

    if (m_dynamicTasks.find(id) == m_dynamicTasks.end())
    {
      Task *newTask = new Task(name, funcPtr);
      newTask->m_data = dataPtr;

      newTask->m_fiber = CreateFiberEx(EngineSettings::FiberStackSize,
        EngineSettings::FiberReservedStackSize, FIBER_FLAG_FLOAT_SWITCH,
        funcPtr, newTask);

      SF_CRITICAL_ERR_EXP(newTask->m_fiber != NULL, "Cannot create fiber");

      newTask->m_taskManager = this;

      m_dynamicTasks[id] = newTask;
    }
    else
      m_dynamicTasks[id]->m_data = dataPtr;
    
    Task *queuedTask = m_dynamicTasks[id];
    queuedTask->m_waitingTaskCounter = &parentTask->m_waitingCounter;
    ++(parentTask->m_waitingCounter.m_counter);

    m_dynamicTasksMutex.unlock();       //Unlock dynamicTasks

    m_queueMutex.lock();                //Lock queue
    m_taskQueue.push_front(queuedTask);
    m_queueMutex.unlock();              //Unlock queue
  }

  Task* TaskManager::_PullAwakeTask(WorkerThread *pullingWorker)
  {
    std::unique_lock<std::mutex> lock(m_awakeTasksMutex);
    Thread handle = pullingWorker->m_threadHandle;
    Task *task = nullptr;

    auto it = m_awakeTasks.find(handle);

    if (it == m_awakeTasks.end())
      return task;

    auto &list = m_awakeTasks[handle];

    if (!list.empty())
    {
      task = list.front();
      list.pop_front();
    }

    return task;
  }

  void TaskManager::_ResumeThreads(void)
  {
    m_workers[0].m_exit = false;

    for (UINT32 i = 1; i < m_numThreads; ++i)
    {
      EnterCriticalSection(&m_workers[i].m_suspendedCS);
      WakeConditionVariable(&m_workers[i].m_suspendedCV);
      LeaveCriticalSection(&m_workers[i].m_suspendedCS);
    }
  }
}