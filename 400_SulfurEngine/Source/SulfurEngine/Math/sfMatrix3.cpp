/******************************************************************************/
/*!
\par     Sulfur
\file    sfMatrix3.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   3x3 Matrix

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfMatrix3.hpp"

namespace Sulfur
{
  const Matrix3 SF_ATTR_ALIGN_16 Matrix3::c_identity = Matrix3(Vector3::c_xAxis,
    Vector3::c_yAxis, Vector3::c_zAxis);
}