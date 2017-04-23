/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector3.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   3D Vector

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfVector3.hpp"

namespace Sulfur
{
  const Vector3 SF_ATTR_ALIGN_16 Vector3::c_zero   = Vector3(0.0, 0.0, 0.0);
  const Vector3 SF_ATTR_ALIGN_16 Vector3::c_xAxis  = Vector3(1.0, 0.0, 0.0);
  const Vector3 SF_ATTR_ALIGN_16 Vector3::c_yAxis  = Vector3(0.0, 1.0, 0.0);
  const Vector3 SF_ATTR_ALIGN_16 Vector3::c_zAxis  = Vector3(0.0, 0.0, 1.0);
}
