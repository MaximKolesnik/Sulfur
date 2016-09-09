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
    thisThread->m_selfFiberHandle = GetCurrentFiber();

    bool flag = false;
    while (!flag)
    {
      ITask *task = thisThread->m_taskManager->PullTask();
      if (task)
      { 
        task->m_worker = thisThread;
        SwitchToFiber(task->m_fiber);
      }

      flag = thisThread->m_taskManager->IsDone();
    }

    ConvertFiberToThread();

    return 0;
  }
}