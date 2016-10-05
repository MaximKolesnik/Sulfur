/******************************************************************************/
/*!
\par     Sulfur
\file    sfTaskManager.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Task manager. Used to update all components and systems
         Main thread is used as a worker

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
  class DependencyGraph;

  class TaskManager
  {
  public:
    TaskManager(void);
    ~TaskManager(void);

    void RunTasks(void);

    void AddNode(const std::string &taskName);
    //Task that is first to update, does not depend on anything
    void SetStartingTask(const std::string &taskName);
    void SetDependency(const std::string &taskName, const std::string &dependsOn);
    void CompleteGraph(void);
    bool IsDone(void) const; //Are all tasks done for this frame

    Task* PullTask(WorkerThread *pullingWorker);

    void EnqueueTask(TaskPtr funcPtr, uintptr_t id, void *dataPtr,
      const std::string &name, Task *parentTask);

  private:
    friend DWORD WINAPI WorkerThreadRoutine(LPVOID lpParam);
    friend WorkerThread;
    friend Task;

    //Map for waking tasks with thread affinity;
    typedef std::unordered_map<Thread, std::list<Task*> > TaskMapThread;

    TaskManager(const TaskManager &) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    void _CreateWorkerThreads(void);
    void _ProcessCompletedTask(Task *task);
    void _ProcessWaitingTask(Task *task);
    Task* _PullAwakeTask(WorkerThread *pullingWorker);
    void _ResumeThreads(void);

    UINT32 m_numThreads; //hardware concurrent threads

    WorkerThread *m_workers;

    std::deque<Task*> m_taskQueue;
    TaskMapThread m_awakeTasks;
    std::list<Task*> m_waitingTasks;

    std::mutex m_queueMutex;
    std::mutex m_waitingTasksMutex;
    std::mutex m_awakeTasksMutex;
    std::mutex m_graphMutex;
    std::mutex m_dynamicTasksMutex;

    //Task instances with no data placement
    std::unordered_map<std::string, Task*> m_taskRegistry;
    //Enqueued tasks with data placement
    std::unordered_map<uintptr_t, Task*> m_dynamicTasks;

    DependencyGraph *m_depGraph;
  };
}
