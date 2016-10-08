/******************************************************************************/
/*!
\par     Sulfur
\file    sfColor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   RGBA color

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Math/sfVector4.hpp"

namespace Sulfur
{
	
  class Color : public Vector4
  {
  
  public:
    Color(Real R = 1.0f, Real G = 1.0f, Real B = 1.0f, Real A = 1.0f);

  };
  
}