/******************************************************************************/
/*!
\par     Sulfur
\file    sfQuaternion.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   Quaternion

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfQuaternion.hpp"
#include "sfMatrix3.hpp"
#include "sfMatrix4.hpp"

namespace Sulfur
{
  SF_VEC_CALL Quaternion::Quaternion(const Matrix3 &m)
  {
    Real trace = m[0][0] + m[1][1] + m[2][2];
    Quaternion q;

    if (trace > 0)
    {
      Real s = MathUtils::Sqrt(Real(trace + 1.0)) * Real(2.0);
      Set(
        (Real)0.25 * s,
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[1][0] - m[0][1]) / s);
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[0][0] - m[1][1] - m[2][2])) * 2;
      Set(
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)0.25 * s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)(m[0][2] + m[2][0]) / s
        );
    }
    else if (m[1][1] > m[2][2])
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[1][1] - m[0][0] - m[2][2])) * 2;
      Set(
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)0.25 * s,
        (Real)(m[1][2] + m[2][1]) / s
        );
    }
    else
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[2][2] - m[0][0] - m[1][1])) * 2;
      Set(
        (Real)(m[1][0] - m[0][1]) / s,
        (Real)(m[0][2] + m[2][0]) / s,
        (Real)(m[1][2] + m[2][1]) / s,
        (Real)0.25 * s
        );
    }
  }

  SF_VEC_CALL Quaternion::Quaternion(const Matrix4 &m)
  {
    Real trace = m[0][0] + m[1][1] + m[2][2];
    Quaternion q;

    if (trace > 0)
    {
      Real s = MathUtils::Sqrt(Real(trace + 1.0)) * Real(2.0);
      Set(
        (Real)0.25 * s,
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[1][0] - m[0][1]) / s);
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[0][0] - m[1][1] - m[2][2])) * 2;
      Set(
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)0.25 * s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)(m[0][2] + m[2][0]) / s
        );
    }
    else if (m[1][1] > m[2][2])
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[1][1] - m[0][0] - m[2][2])) * 2;
      Set(
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)0.25 * s,
        (Real)(m[1][2] + m[2][1]) / s
        );
    }
    else
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[2][2] - m[0][0] - m[1][1])) * 2;
      Set(
        (Real)(m[1][0] - m[0][1]) / s,
        (Real)(m[0][2] + m[2][0]) / s,
        (Real)(m[1][2] + m[2][1]) / s,
        (Real)0.25 * s
        );
    }
  }

  Matrix3 SF_VEC_CALL Quaternion::GetMatrix3(void) const
  {
    return Matrix3(*this);
  }

  Matrix4 SF_VEC_CALL Quaternion::GetMatrix4(void) const
  {
    return Matrix4(*this);
  }

  void SF_VEC_CALL Quaternion::Set(const Matrix3 &m)
  {
    Real trace = m[0][0] + m[1][1] + m[2][2];
    Quaternion q;

    if (trace > 0)
    {
      Real s = MathUtils::Sqrt(Real(trace + 1.0)) * Real(2.0);
      Set(
        (Real)0.25 * s,
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[1][0] - m[0][1]) / s);
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[0][0] - m[1][1] - m[2][2])) * 2;
      Set(
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)0.25 * s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)(m[0][2] + m[2][0]) / s
        );
    }
    else if (m[1][1] > m[2][2])
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[1][1] - m[0][0] - m[2][2])) * 2;
      Set(
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)0.25 * s,
        (Real)(m[1][2] + m[2][1]) / s
        );
    }
    else
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[2][2] - m[0][0] - m[1][1])) * 2;
      Set(
        (Real)(m[1][0] - m[0][1]) / s,
        (Real)(m[0][2] + m[2][0]) / s,
        (Real)(m[1][2] + m[2][1]) / s,
        (Real)0.25 * s
        );
    }
  }

  void SF_VEC_CALL Quaternion::Set(const Matrix4 &m)
  {
    Real trace = m[0][0] + m[1][1] + m[2][2];
    Quaternion q;

    if (trace > 0)
    {
      Real s = MathUtils::Sqrt(Real(trace + 1.0)) * Real(2.0);
      Set(
        (Real)0.25 * s,
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[1][0] - m[0][1]) / s);
    }
    else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[0][0] - m[1][1] - m[2][2])) * 2;
      Set(
        (Real)(m[2][1] - m[1][2]) / s,
        (Real)0.25 * s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)(m[0][2] + m[2][0]) / s
        );
    }
    else if (m[1][1] > m[2][2])
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[1][1] - m[0][0] - m[2][2])) * 2;
      Set(
        (Real)(m[0][2] - m[2][0]) / s,
        (Real)(m[0][1] + m[1][0]) / s,
        (Real)0.25 * s,
        (Real)(m[1][2] + m[2][1]) / s
        );
    }
    else
    {
      Real s = MathUtils::Sqrt(Real(1.0 + m[2][2] - m[0][0] - m[1][1])) * 2;
      Set(
        (Real)(m[1][0] - m[0][1]) / s,
        (Real)(m[0][2] + m[2][0]) / s,
        (Real)(m[1][2] + m[2][1]) / s,
        (Real)0.25 * s
        );
    }
  }
}