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
#include "Settings/sfEngineSettings.hpp"

namespace Sulfur
{
	
  class Time
  {
  
  public:
    Time(Real frameRate = EngineSettings::DefaultFPS);

    void WaitForFrameRate();
    Real GetDt();

  private:
    LARGE_INTEGER m_startCycle;
    Real m_dt;
    Real m_frameRate;
    Real m_frequency;

  };
  
}