/******************************************************************************/
/*!
\par     Sulfur
\file    sfColor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   RGBA color

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfColor.hpp"

namespace Sulfur
{

  Color::Color(Real R, Real G, Real B, Real A)
    : Vector4(R, G, B, A)
  {
  }

}
