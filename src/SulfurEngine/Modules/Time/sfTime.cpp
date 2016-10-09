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

namespace Sulfur
{

  Time::Time(Real frameRate)
    : m_frameRate(frameRate), m_dt(0.0f)
  {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    m_frequency = (Real)frequency.QuadPart;
    QueryPerformanceCounter(&m_startCycle);
  }

  void Time::WaitForFrameRate()
  {
    LARGE_INTEGER currentCycle;
    do
    {
      QueryPerformanceCounter(&currentCycle);
      m_dt = (currentCycle.QuadPart - m_startCycle.QuadPart) / m_frequency;
    } 
    while (1.0f / m_dt < m_frameRate);

    m_startCycle = currentCycle;
  }

  Real Time::GetDt()
  {
    return m_dt;
  }

}
