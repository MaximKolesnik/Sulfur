/******************************************************************************/
/*!
\par     Sulfur
\file    sfTimer.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/4/2016

\brief   Timer. Mainly used as fps controller

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <string>

#include "sfTimer.hpp"
#include "../../Error/sfError.hpp"
#include "../../Settings/EngineSettings.h"

namespace Sulfur
{
  Timer::Timer(void)
  {
#ifdef _DEBUG
    m_initialized = false;
#endif
  }

  Timer::~Timer(void)
  {

  }

  void Timer::Initialize(void)
  {
    m_initialized = true;
    
    BOOL error = QueryPerformanceFrequency(&m_frequency);
    SF_CRITICAL_ERR_EXP(error != 0, std::to_string(GetLastError()));

    m_lastElapsedTimeMs = EngineSettings::DefaultFPS;

    m_secsPerCount = Real(1.0) / m_frequency.QuadPart;
  }

  void Timer::Start(void)
  {
#ifdef _DEBUG
    SF_ASSERT(m_initialized, "Timer is not initialized");
#endif

    BOOL error = QueryPerformanceCounter(&m_lastStart);
    SF_CRITICAL_ERR_EXP(error != 0, std::to_string(GetLastError()));
  }

  void Timer::Stop(void)
  {
#ifdef _DEBUG
    SF_ASSERT(m_initialized, "Timer is not initialized");
#endif

    BOOL error = QueryPerformanceCounter(&m_lastStop);
    SF_CRITICAL_ERR_EXP(error != 0, std::to_string(GetLastError()));

    m_lastElapsedTime.QuadPart = m_lastStop.QuadPart - m_lastStart.QuadPart;
    SF_ASSERT(m_lastElapsedTime.QuadPart >= 0, "Timer was not started");

    m_lastElapsedTimeMs = (m_lastElapsedTime.QuadPart * 1000) * m_secsPerCount;
  }

  Real Timer::GetStartTime(void) const
  {
    return (m_lastStart.QuadPart * 1000) * m_secsPerCount;
  }

  Real Timer::GetStopTime(void) const
  {
    return (m_lastStop.QuadPart * 1000) * m_secsPerCount;
  }

  Real Timer::GetLastDt(void) const
  {
    return m_lastElapsedTimeMs;
  }
}