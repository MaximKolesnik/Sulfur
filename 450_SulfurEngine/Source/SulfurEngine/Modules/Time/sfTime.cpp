/******************************************************************************/
/*!
\par     Sulfur
\file    sfTime.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/8/2016

\brief   Time manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTime.hpp"
#include "Settings/sfEngineSettings.hpp"

namespace Sulfur
{

  Time::Time()
    : m_frameRate(EngineSettings::DefaultFPS), m_dt(0.0f), m_paused(false)
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    m_frequency = (Real)frequency.QuadPart;
    QueryPerformanceCounter(&m_startCycle);
  }

  Time::~Time()
  {

  }

  void Time::WaitForFrameRate()
  {
    LARGE_INTEGER currentCycle;
    do
    {
      QueryPerformanceCounter(&currentCycle);
      m_dt = (currentCycle.QuadPart - m_startCycle.QuadPart) / m_frequency;
    } 
    while (m_dt < m_frameRate);

    if (m_dt > m_frameRate)
      m_dt = EngineSettings::DefaultFPS;

    m_startCycle = currentCycle;
  }

  Real Time::GetDt()
  {
    return m_dt;
  }

  void Time::SetPaused(bool paused)
  {
    m_paused = paused;
  }

  bool Time::IsPaused() const
  {
    return m_paused;
  }

}
