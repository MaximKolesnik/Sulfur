/******************************************************************************/
/*!
\par     Sulfur
\file    sfWorker.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Worker

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <mutex>

#include "sfTaskManager.hpp"
#include "sfWorker.hpp"

namespace Sulfur
{
  DWORD WINAPI WorkerThreadRoutine(LPVOID lpParam)
  {
    WorkerThread *thisThread = reinterpret_cast<WorkerThread*>(lpParam);
    
    ConvertThreadToFiber(NULL);

    while (true)
    {
      if (thisThread->m_coreAffinity != 0)
      {
        EnterCriticalSection(&thisThread->m_suspendedCS);
        SleepConditionVariableCS(&thisThread->m_suspendedCV, &thisThread->m_suspendedCS, INFINITE);
        LeaveCriticalSection(&thisThread->m_suspendedCS);
      }

      if (thisThread->m_exit)
        break;

      thisThread->m_selfFiberHandle = GetCurrentFiber();

      while (true)
      {
        Task *task = thisThread->m_taskManager->PullTask(thisThread);
        if (task)
        {
          task->m_executingWorker = thisThread;
          task->m_done = false;
          task->m_waiting = false;

          SwitchToFiber(task->m_fiber);

          if (task->m_done)
            thisThread->m_taskManager->_ProcessCompletedTask(task);
          else if (task->m_waiting)
            thisThread->m_taskManager->_ProcessWaitingTask(task);

          if (thisThread->m_taskManager->IsDone())
            break;
        }
      }

      if (thisThread->m_coreAffinity == 0)
        thisThread->m_exit = true;
    }

    ConvertFiberToThread();

    return 0;
  }
}