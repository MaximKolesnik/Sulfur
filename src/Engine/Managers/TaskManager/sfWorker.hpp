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

#include <thread>

namespace Sulfur
{
  class Worker
  {
  public:
    Worker(void);
    void operator()(void);

    std::thread::id GetThreadId(void) const { return std::this_thread::get_id(); }

   private:
  };
}