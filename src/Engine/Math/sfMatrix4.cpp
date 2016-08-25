/******************************************************************************/
/*!
\par     Sulfur
\file    sfMatrix4.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   4x4 Matrix

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfMatrix4.hpp"
#include "sfVector4.hpp"

namespace Sulfur
{
  const Matrix4 SF_ATTR_ALIGN_16 Matrix4::c_identity
    = Matrix4(Real(1.0), Real(0.0), Real(0.0), Real(0.0),
      Real(0.0), Real(1.0), Real(0.0), Real(0.0),
      Real(0.0), Real(0.0), Real(1.0), Real(0.0),
      Real(0.0), Real(0.0), Real(0.0), Real(1.0));
}