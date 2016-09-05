/******************************************************************************/
/*!
\par     Sulfur
\file    sfTimer.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/4/2016

\brief   Timer. Mainly used as fps controller

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <WinBase.h>

#include "../../Types/sfTypes.hpp"

namespace Sulfur
{
  class Timer
  {
  public:
    Timer(void);
    ~Timer(void);

    void Initialize(void);

    void Start(void);
    void Stop(void);

    //Returns time the clock started in ms
    Real GetStartTime(void) const;
    //Returns time the clock stopped in ms
    Real GetStopTime(void) const;

    //Ms
    Real GetLastDt(void) const;
  private:
    Timer(const Timer&) = delete;
    const Timer& operator=(const Timer&) = delete;

    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_lastStart;
    LARGE_INTEGER m_lastStop;

    LARGE_INTEGER m_lastElapsedTime;
    Real m_lastElapsedTimeMs;

    Real m_secsPerCount;
#ifdef _DEBUG
    bool m_initialized;
#endif
  };
}