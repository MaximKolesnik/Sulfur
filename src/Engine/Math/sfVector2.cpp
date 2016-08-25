/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector2.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   2D Vector

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfVector2.hpp"

namespace Sulfur
{
  const Vector2 SF_ATTR_ALIGN_16 Vector2::c_zero = Vector2(0.0f, 0.0f);
  const Vector2 SF_ATTR_ALIGN_16 Vector2::c_xAxis = Vector2(1.0f, 0.0f);
  const Vector2 SF_ATTR_ALIGN_16 Vector2::c_yAxis = Vector2(0.0f, 1.0f);
}
