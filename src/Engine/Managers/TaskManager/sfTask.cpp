/******************************************************************************/
/*!
\par     Sulfur
\file    sfTask.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief   Task

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfTask.hpp"
#include "sfTaskManager.hpp"

namespace Sulfur
{
  VOID CALLBACK FiberRoutine(PVOID lpParam)
  {
    ITask *task = reinterpret_cast<ITask*>(GetFiberData());

    task->Execute();

    task->m_worker->m_taskManager->_ProcessCompletedTask(task);
    SwitchToFiber(task->m_worker->m_selfFiberHandle);
    Fib
  }
}