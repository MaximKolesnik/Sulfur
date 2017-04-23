/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector4.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   4D Vector

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfVector4.hpp"

namespace Sulfur
{
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_zero = Vector4(0.0, 0.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_xAxis = Vector4(1.0, 0.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_yAxis = Vector4(0.0, 1.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_zAxis = Vector4(0.0, 0.0, 1.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_wAxis = Vector4(0.0, 0.0, 0.0, 1.0);
}