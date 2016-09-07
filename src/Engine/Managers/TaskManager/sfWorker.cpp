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
  Worker::Worker(void)
  {

  }

  void Worker::operator()(void)
  {
    TaskManager *taskManager = TaskManager::Instance();

    while (true)
    {
      std::unique_lock<std::mutex> lock(taskManager->m_queueMutex); //lock queue

      while (taskManager->m_taskQueue.empty())
        taskManager->m_stopCondition.wait(lock);

      ITask *task = taskManager->m_taskQueue.front();
      taskManager->m_taskQueue.pop_front();

      lock.release();                                               //release queue

      (*task)();
    }
  }
}