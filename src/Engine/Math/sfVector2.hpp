/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector2.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   2D Vector

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <limits>

#include "../sfProjectDefs.hpp"
#include "sfMathDefs.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  SF_ATTR_ALIGN_16 class Vector2
  {
  public:
    static const Vector2 SF_ATTR_ALIGN_16 c_zero;
    static const Vector2 SF_ATTR_ALIGN_16 c_xAxis;
    static const Vector2 SF_ATTR_ALIGN_16 c_yAxis;

    SF_FORCE_INLINE SF_VEC_CALL Vector2(void)
    {

    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Vector2(Real x, Real y)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(0.0, 0.0, y, x);
#else
      m_comps[0] = x;
      m_comps[1] = y;
      m_comps[2] = m_comps[3] = 0.0;
#endif
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Vector2(__m128 data)
    {
      m_data = data;
    }
#endif

    //Getters
    SF_FORCE_INLINE Real SF_VEC_CALL GetX(void) const
    {
      return m_comps[0];
    }

    SF_FORCE_INLINE Real SF_VEC_CALL GetY(void) const
    {
      return m_comps[1];
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE __m128 SF_VEC_CALL Get128(void) const
    {
      return m_data;
    }
#endif

    //Setters
    SF_FORCE_INLINE void SF_VEC_CALL SetX(Real x)
    {
      m_comps[0] = x;
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetY(Real y)
    {
      m_comps[1] = y;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real x, Real y)
    {
      m_comps[0] = x;
      m_comps[1] = y;
    }

    //Compute dot product of two vectors
    SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector2 &other) const
    {
#ifdef SF_USE_SIMD
      Vector2 mult(_mm_mul_ps(m_data, other.m_data));

      return mult.m_comps[0] + mult.m_comps[1];
#else
      return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1];
  #endif
    }

    //Compute length of the vector
    SF_FORCE_INLINE Real SF_VEC_CALL Length(void) const
    {
      return MathUtils::Sqrt(this->Dot(*this));
    }

    //Compute squared length of the vector
    SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(void) const
    {
      return this->Dot(*this);
    }

    //Normalize the vector
    SF_FORCE_INLINE Vector2& SF_VEC_CALL Normalize(void)
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this /= Length();
    }

    //Get normalized vector
    SF_FORCE_INLINE Vector2 SF_VEC_CALL Normalized(void) const
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this / Length();
    }

    //Set vector components to zero
    SF_FORCE_INLINE void SF_VEC_CALL ZeroOut(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_setzero_ps();
 #else
      m_comps[0] = 0.0;
      m_comps[1] = 0.0;
#endif
    }

    SF_FORCE_INLINE bool SF_VEC_CALL IsZero(void) const
    {
      return (*this == c_zero);
    }

    SF_FORCE_INLINE bool SF_VEC_CALL IsZeroEpsilon(void) const
    {
      return this->LengthSq() < SF_EPSILON * SF_EPSILON;
    }

    //Check if any of components has infinity
    SF_FORCE_INLINE bool SF_VEC_CALL HasInfinity(void) const
    {
#ifdef SF_USE_SIMD
      __m128 test = _mm_andnot_ps(c_SIMDZeroMask, m_data);
      __m128 inf = _mm_set1_ps(std::numeric_limits<Real>::infinity());

      test = _mm_cmp_ps(test, inf, _CMP_EQ_OQ);

      return _mm_movemask_ps(test) != 0;
#else
      const Real inf = std::numeric_limits<Real>::infinity();

      return (m_comps[0] == inf) || (m_comps[1] == inf);
#endif
    }

    //Return a vector with absolute values of each component
    SF_FORCE_INLINE Vector2 SF_VEC_CALL GetAbs(void) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_and_ps(m_data, c_SIMDAbsMask));
#else
      return Vector2(MathUtils::Abs(m_comps[0]), MathUtils::Abs(m_comps[1]));
#endif
    }

    //Performs abs operation on the current vector
    SF_FORCE_INLINE Vector2& SF_VEC_CALL Abs(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_and_ps(m_data, c_SIMDAbsMask);
#else
      m_comps[0] = MathUtils::Abs(m_comps[0]);
      m_comps[1] = MathUtils::Abs(m_comps[1]);
#endif
      return *this;
    }

    //Returns a negated vector
    SF_FORCE_INLINE Vector2 SF_VEC_CALL Negated(void) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_xor_ps(m_data, c_SIMDNegMask));
#else
      return Vector2(-m_comps[0], -m_comps[1]);
#endif
    }

    //Negates the current vector
    SF_FORCE_INLINE Vector2& SF_VEC_CALL Negate(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_xor_ps(m_data, c_SIMDNegMask);
#else
      m_comps[0] = -m_comps[0];
      m_comps[1] = -m_comps[1];
#endif
      return *this;
    }

    //Return distance between two points
    SF_FORCE_INLINE Real SF_VEC_CALL Distance(const Vector2 &other) const
    {
      return (*this - other).Length();
    }

    SF_FORCE_INLINE Real SF_VEC_CALL DistanceSq(const Vector2 &other) const
    {
      return (*this - other).LengthSq();
    }

    //Return the index of the max axis
    SF_FORCE_INLINE int SF_VEC_CALL MaxAxis(void) const
    {
      return m_comps[0] < m_comps[1] ? 1 : 0;
    }

    //Return the index of the min axis
    SF_FORCE_INLINE int SF_VEC_CALL MinAxis(void) const
    {
      return m_comps[0] < m_comps[1] ? 0 : 1;
    }

    //Return the value of the max axis
    SF_FORCE_INLINE Real MaxAxisValue(void) const
    {
      return m_comps[0] < m_comps[1] ? m_comps[1] : m_comps[0];
    }

    //Return the value of the min axis
    SF_FORCE_INLINE Real SF_VEC_CALL MinAxisValue(void) const
    {
      return m_comps[0] < m_comps[1] ? m_comps[0] : m_comps[1];
    }

    //Linear interpolation
    SF_FORCE_INLINE Vector2 SF_VEC_CALL Lerp(const Vector2 &other, Real t) const
    {
#ifdef SF_USE_SIMD
      __m128 mt = _mm_load_ss(&t);
      mt = _mm_shuffle_ps(mt, mt, 0x40); //_MM_SHUFFLE(1, 0, 0, 0);

      __m128 lerpDir = _mm_sub_ps(other.m_data, m_data);
      __m128 lerpMove = _mm_mul_ps(lerpDir, mt);

      return Vector2(_mm_add_ps(m_data, lerpMove));
#else
      Vector2 lerpDir = other - *this;

      return (*this) + lerpDir * t;
#endif
    }

    //Get rotated vector
    SF_FORCE_INLINE Vector2 SF_VEC_CALL RotatedRad(Real angleRad) const
    {
      Real sinVal = MathUtils::Sin(angleRad);
      Real cosVal = MathUtils::Cos(angleRad);

#ifdef SF_USE_SIMD
      __m128 mSin = _mm_load_ss(&sinVal);
      mSin = _mm_shuffle_ps(mSin, mSin, 0x40);
      mSin = _mm_xor_ps(mSin, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));

      __m128 mCos = _mm_load_ss(&cosVal);
      mCos = _mm_shuffle_ps(mCos, mCos, 0x40);

      __m128 cosMult = _mm_mul_ps(m_data, mCos);
      __m128 sinMult = _mm_mul_ps(m_data, mSin);

      sinMult = _mm_shuffle_ps(sinMult, sinMult, 0x1);

      return Vector2(_mm_add_ps(cosMult, sinMult));
#else
      return Vector2(cosVal * m_comps[0] - sinVal * m_comps[1],
                      sinVal * m_comps[0] + cosVal * m_comps[1]);
#endif
    }

    //Rotate current vector
    SF_FORCE_INLINE Vector2& SF_VEC_CALL RotateRad(Real angleRad)
    {
#ifdef SF_USE_SIMD
      m_data = this->RotatedRad(angleRad).m_data;
#else
      Real sinVal = std::sin(angleRad);
      Real cosVal = std::cos(angleRad);

      Real tempX = cosVal * m_comps[0] - sinVal * m_comps[1];
      m_comps[1] = sinVal * m_comps[0] + cosVal * m_comps[1];
      m_comps[0] = tempX;
#endif

      return *this;
    }

    SF_FORCE_INLINE Vector2 SF_VEC_CALL RotatedDeg(Real angleDeg) const
    {
      return this->RotatedRad(angleDeg * SF_RADS_PER_DEG);
    }

    SF_FORCE_INLINE Vector2& SF_VEC_CALL RotateDeg(Real angleDeg)
    {
      return this->RotateRad(angleDeg * SF_RADS_PER_DEG);
    }

    //Get angle between two vectors
    SF_FORCE_INLINE Real SF_VEC_CALL GetAngle(const Vector2 &other) const
    {
      Real scalar = MathUtils::Sqrt(this->LengthSq() * other.LengthSq());
      SF_ASSERT(scalar != 0, "One of the vectors has length 0");
      return MathUtils::Acos(this->Dot(other) / scalar);
    }

    //Summation operators
    SF_FORCE_INLINE Vector2 SF_VEC_CALL operator+(const Vector2 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_add_ps(m_data, other.m_data));
#else
      return Vector2(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1]);
#endif
    }

    SF_FORCE_INLINE Vector2& SF_VEC_CALL operator+=(const Vector2 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_add_ps(m_data, other.m_data);
#else
      m_comps[0] += other.m_comps[0];
      m_comps[1] += other.m_comps[1];
#endif
      return *this;
    }

    //Negation operator
    SF_FORCE_INLINE Vector2 SF_VEC_CALL operator-(void) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_xor_ps(m_data, c_SIMDNegMask));
#else
      return Vector2(-m_comps[0], -m_comps[1]);
#endif
    }

    //Subtraction operators
    SF_FORCE_INLINE Vector2 SF_VEC_CALL operator-(const Vector2 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_sub_ps(m_data, other.m_data));
#else
      return Vector2(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1]);
#endif
    }

    SF_FORCE_INLINE Vector2& SF_VEC_CALL operator-=(const Vector2 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_sub_ps(m_data, other.m_data);
#else
      m_comps[0] -= other.m_comps[0];
      m_comps[1] -= other.m_comps[1];
#endif
      return *this;
    }

    //Scaling operators
    SF_FORCE_INLINE Vector2 SF_VEC_CALL operator*(Real scalar) const
    {
#ifdef SF_USE_SIMD
      return Vector2(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector2(m_comps[0] * scalar, m_comps[1] * scalar);
#endif
    }

    SF_FORCE_INLINE Vector2& SF_VEC_CALL operator*=(Real scalar)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] *= scalar;
      m_comps[1] *= scalar;
#endif
      return *this;
    }

    SF_FORCE_INLINE Vector2 SF_VEC_CALL operator/(Real scalar) const
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");

#ifdef SF_USE_SIMD
      return Vector2(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector2(m_comps[0] / scalar, m_comps[1] / scalar);
#endif
    }

    SF_FORCE_INLINE Vector2& SF_VEC_CALL operator/=(Real scalar)
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");

#ifdef SF_USE_SIMD
      m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] /= scalar;
      m_comps[1] /= scalar;
#endif
      return *this;
    }

    //Index access
    SF_FORCE_INLINE Real& SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(i >= 0 && i < 2, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE const Real& SF_VEC_CALL operator[](int i) const
    {
      SF_ASSERT(i >= 0 && i < 2, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator==(const Vector2 &other) const
    {
#ifdef SF_USE_SIMD
      return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
      return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1]);
#endif
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator!=(const Vector2 &other) const
    {
      return !(*this == other);
    }

  private:
#ifdef SF_USE_SIMD
    union
    {
      Real m_comps[4];
      __m128 m_data;
    };
#else
    Real m_comps[4];
#endif
  };

  SF_FORCE_INLINE Vector2 SF_VEC_CALL operator*(Real s, const Vector2 &v)
  {
    return v * s;
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector2 &v1, const Vector2 &v2)
  {
    return v1.Dot(v2);
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Length(const Vector2 &v)
  {
    return v.Length();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(const Vector2 &v)
  {
    return v.LengthSq();
  }

  SF_FORCE_INLINE Vector2& SF_VEC_CALL Normalize(Vector2 &v)
  {
    return v.Normalize();
  }

  SF_FORCE_INLINE Vector2& SF_VEC_CALL Abs(Vector2 &v)
  {
    return v.Abs();
  }

  SF_FORCE_INLINE Vector2& SF_VEC_CALL Negate(Vector2 &v)
  {
    return v.Negate();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Distance(const Vector2 &v1, const Vector2 &v2)
  {
    return v1.Distance(v2);
  }

  SF_FORCE_INLINE Real SF_VEC_CALL DistanceSq(const Vector2 &v1, const Vector2 &v2)
  {
    return v1.DistanceSq(v2);
  }

  SF_FORCE_INLINE int SF_VEC_CALL MaxAxis(const Vector2 &v)
  {
    return v.MaxAxis();
  }

  SF_FORCE_INLINE int SF_VEC_CALL MinAxis(const Vector2 &v)
  {
    return v.MinAxis();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL MaxAxisValue(const Vector2 &v)
  {
    return v.MaxAxisValue();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL MinAxisValue(const Vector2 &v)
  {
    return v.MinAxisValue();
  }

  SF_FORCE_INLINE Vector2 SF_VEC_CALL Lerp(const Vector2 &start, const Vector2 &end, Real t)
  {
    return start.Lerp(end, t);
  }

  SF_FORCE_INLINE Vector2& SF_VEC_CALL RotateRad(Vector2 &v, Real angleRad)
  {
    return v.RotateRad(angleRad);
  }

  SF_FORCE_INLINE Vector2& SF_VEC_CALL RotateDeg(Vector2 &v, Real angleDeg)
  {
    return v.RotateDeg(angleDeg);
  }
}
