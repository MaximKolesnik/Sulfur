#pragma once

#include <immintrin.h>
#include <cfloat>
#include <cmath>

#include "../sfProjectDefs.hpp"
#include "../Types/sfTypes.hpp"

#pragma warning(disable : 4166)

namespace Sulfur
{
  #ifdef SF_USE_SIMD

    const __m128 c_SIMDZeroMask = _mm_set_ps(-0.0f, -0.0f, -0.0f, -0.0f);
    const __m128 c_SIMDAbsMask  = _mm_castsi128_ps(_mm_set_epi32(0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));
    const __m128 c_SIMDNegMask  = _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x80000000, 0x80000000, 0x80000000));

  #endif  //SF_USE_SIMD

  #define SF_EPSILON  Sulfur::Real(0.000001)
#ifdef SF_USE_SIMD
#define SF_UT_EPSILON SF_EPSILON
#else
#define SF_UT_EPSILON Sulfur::Real(0.02) //There are some precision issues, when using unit test both for Simd and scalar math
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

    SF_FORCE_INLINE Real Abs(Real real)
    {
      return std::abs(real);
    }
  }
}
