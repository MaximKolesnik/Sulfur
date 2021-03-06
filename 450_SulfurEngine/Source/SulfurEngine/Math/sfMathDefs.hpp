/******************************************************************************/
/*!
\par     Sulfur
\file    sfMathDefs.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   Masks and definitions for math library

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <immintrin.h>
#include <cfloat>
#include <cmath>

#include "../sfProjectDefs.hpp"
#include "../Types/sfTypes.hpp"

// Illegal calling convention for ctor/dtor
#pragma warning(disable : 4166)

namespace Sulfur
{
#ifndef SF_USE_DOUBLE_PRECISION
#define SF_REAL_MAX FLT_MAX
#else
#define SF_REAL_MAX DBL_MAX
#endif

  #ifdef SF_USE_SIMD

    const __m128 c_SIMDZeroMask = _mm_set_ps(-0.0f, -0.0f, -0.0f, -0.0f);
    const __m128 c_SIMDAbsMask  = _mm_castsi128_ps(_mm_set_epi32(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));
    const __m128 c_SIMDNegMask  = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000));

  #endif  //SF_USE_SIMD

  #define SF_EPSILON  Sulfur::Real(0.000001)
  #define SF_EPSILON_NUM_DECIMALS 6
#ifdef SF_USE_SIMD
#define SF_UT_EPSILON SF_EPSILON
#else
#define SF_UT_EPSILON Sulfur::Real(0.02) 
//There are some precision issues when using unit test both for Simd and scalar math
//Since we are not going to use scalar in Release we want to make sure that at least works
#endif


  #define SF_PI           Sulfur::Real(3.1415926535897932384626433832795029)
  #define SF_2PI          (Sulfur::Real(2.0 * SF_PI))
  #define SF_RADS_PER_DEG (SF_PI / Sulfur::Real(180.0))
  #define SF_DEGS_PER_RAD (Sulfur::Real(180.0) / SF_PI)

  namespace MathUtils
  { 
    SF_FORCE_INLINE Real Sqrt(Real real)
    {
      return std::sqrt(real);
    }

    SF_FORCE_INLINE Real Sin(Real real)
    {
      return std::sin(real);
    }

    SF_FORCE_INLINE Real Cos(Real real)
    {
      return std::cos(real);
    }

    SF_FORCE_INLINE Real Acos(Real real)
    {
      return std::acos(real);
    }

    SF_FORCE_INLINE Real Atan2(Real real1, Real real2)
    {
      return std::atan2(real1, real2);
    }

    SF_FORCE_INLINE Real Abs(Real real)
    {
      return std::abs(real);
    }

    SF_FORCE_INLINE Real Log2(Real real)
    {
      return std::log2(real);
    }

    SF_FORCE_INLINE Real Clamp(Real r, Real min, Real max)
    {
      if (r < min)
        return min;
      if (r > max)
        return max;
      return r;
    }

    template <class Type>
    SF_FORCE_INLINE Type GetSign(Type val)
    {
      return (val >= Type(0.0)) ? Type(1.0) : Type(-1.0);
    }

    SF_FORCE_INLINE Real Round(Real val, UINT16 numDecimals)
    {
      SF_ASSERT(numDecimals != 0, "Rounding error");

      int mul = (int)pow(10, numDecimals);

      return roundf(val * mul) / mul;
    }
  }
}
