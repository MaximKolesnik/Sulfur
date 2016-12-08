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
#pragma once

#include "Types\sfSingleton.hpp"

namespace Sulfur
{
	
  class Time
  {
    SF_SINGLETON(Time);
  public:

    void WaitForFrameRate();
    Real GetDt();

    void SetPaused(bool paused);
    bool IsPaused() const;

  private:
    LARGE_INTEGER m_startCycle;
    Real m_dt;
    Real m_frameRate;
    Real m_frequency;

    bool m_paused;

  };
  
}