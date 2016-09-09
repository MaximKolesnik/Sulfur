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

#pragma once

#include <WinBase.h>


namespace Sulfur
{
  class TaskManager;

  typedef HANDLE Thread;
  typedef HANDLE Fiber;

  struct WorkerThread
  {
    Thread m_threadHandle = NULL;
    Fiber  m_selfFiberHandle = NULL;

    UINT32 m_coreAffinity = -1;

    TaskManager *m_taskManager = nullptr;
  };

  DWORD WINAPI WorkerThreadRoutine(LPVOID lpParam);
}