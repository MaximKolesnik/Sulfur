/******************************************************************************/
/*!
\par     Sulfur
\file    sfQuaternion.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   Quaternion

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <iostream>

#include "../sfProjectDefs.hpp"
#include "sfMathDefs.hpp"
#include "sfVector3.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  SF_ATTR_ALIGN_16 class Quaternion
  {
  public:
    SF_FORCE_INLINE SF_VEC_CALL Quaternion(void)
    {

    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Quaternion(Real q0, Real q1, Real q2,
                                                    Real q3)
    {
      Set(q0, q1, q2, q3);
    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Quaternion(const Vector3 &axis,
                                                    Real angleRad)
    {
      SetRotation(axis, angleRad);
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Quaternion(__m128 vec)
    {
      m_data = vec;
    }

    SF_FORCE_INLINE __m128 SF_VEC_CALL Get128(void) const
    {
      return m_data;
    }
#endif

    SF_FORCE_INLINE void SF_VEC_CALL SetRotation(const Vector3 &axis, Real angleRad)
    {
      Real axisLength = axis.Length();

      SF_ASSERT(axisLength != 0.0, "Axis has length of 0");

      Real scalar = MathUtils::Sin(angleRad * Real(0.5)) / axisLength;

      Set(MathUtils::Cos(angleRad * Real(0.5)), axis.GetX() * scalar, axis.GetY() * scalar,
          axis.GetZ() * scalar);
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real q0, Real q1, Real q2, Real q3)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(q3, q2, q1, q0);
#else
      m_comps[0] = q0, m_comps[1] = q1, m_comps[2] = q2, m_comps[3] = q3;
#endif
    }

    //Invert the quaternion
    SF_FORCE_INLINE Quaternion& SF_VEC_CALL Invert(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_xor_ps(m_data, _mm_set_ps(+0.0f, -0.0f, -0.0f, -0.0f));
#else
      m_comps[1] = -m_comps[1], m_comps[2] = -m_comps[2], m_comps[3] = -m_comps[3];
#endif

      return *this;
    }

    //Return inverted copy of the quaternion
    SF_FORCE_INLINE Quaternion SF_VEC_CALL Inverted(void) const
    {
#ifdef SF_USE_SIMD
      return Quaternion(_mm_xor_ps(m_data, _mm_set_ps(+0.0f, -0.0f, -0.0f, -0.0f)));
#else
      return Quaternion(m_comps[0], -m_comps[1], -m_comps[2], -m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Quaternion &other) const
    {
#ifdef SF_USE_SIMD
      __m128 mul = _mm_mul_ps(m_data, other.m_data);
      __m128 sum1 = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, 0xE)); //_MM_SHUFFLE(0, 0, 3, 2)
      __m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1)); //_MM_SHUFFLE(0, 0, 0, 1)

      return _mm_cvtss_f32(sum);
#else
      return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
        + m_comps[2] * other.m_comps[2] + m_comps[3] * other.m_comps[3];
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Length(void) const
    {
#ifdef SF_USE_SIMD
      __m128 qMul = _mm_mul_ps(m_data, m_data);
      __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE)); //_MM_SHUFFLE(0, 0, 3, 2)
      __m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1)); //_MM_SHUFFLE(0, 0, 0, 1)
      
      sum = _mm_sqrt_ps(sum);

      return _mm_cvtss_f32(sum);
#else
      return MathUtils::Sqrt(m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
      + m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(void) const
    {
#ifdef SF_USE_SIMD
      __m128 qMul = _mm_mul_ps(m_data, m_data);
      __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE)); //_MM_SHUFFLE(0, 0, 3, 2)
      __m128 sum = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1)); //_MM_SHUFFLE(0, 0, 0, 1)

      return _mm_cvtss_f32(sum);
#else
      return m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
        + m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3];
#endif
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL Normalized(void) const
    {
      SF_ASSERT(this->Length() != 0, "Normalization of zero length Quaternion");
#ifdef SF_USE_SIMD
      __m128 qMul = _mm_mul_ps(m_data, m_data);
      __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE)); //_MM_SHUFFLE(0, 0, 3, 2)
      __m128 length = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1)); //_MM_SHUFFLE(0, 0, 0, 1)
      length = _mm_sqrt_ps(length);
      length = _mm_rcp_ps(length);
      
      return Quaternion(_mm_mul_ps(length, m_data));
#else
      Real length = Real(1.0) / (m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
        + m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3]);

      return Quaternion(m_comps[0] * length, m_comps[1] * length,
        m_comps[2] * length, m_comps[3] * length);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL Normalize(void)
    {
      SF_ASSERT(this->Length() != 0, "Normalization of zero length Quaternion");
#ifdef SF_USE_SIMD
      __m128 qMul = _mm_mul_ps(m_data, m_data);
      __m128 sum1 = _mm_add_ps(qMul, _mm_shuffle_ps(qMul, qMul, 0xE)); //_MM_SHUFFLE(0, 0, 3, 2)
      __m128 length = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, 0x1)); //_MM_SHUFFLE(0, 0, 0, 1)
      length = _mm_sqrt_ps(length);
      length = _mm_rcp_ps(length);

      m_data = _mm_mul_ps(length, m_data);
#else
      Real length = Real(1.0) / (m_comps[0] * m_comps[0] + m_comps[1] * m_comps[1]
        + m_comps[2] * m_comps[2] + m_comps[3] * m_comps[3]);

      m_comps[0] *= length, m_comps[1] *= length,
        m_comps[2] *= length, m_comps[3] *= length;
#endif
      return *this;
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator+(const Quaternion &other) const
    {
#ifdef SF_USE_SIMD
      return Quaternion(_mm_add_ps(m_data, other.m_data));
#else
      return Quaternion(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
          m_comps[2] + other.m_comps[2], m_comps[3] + other.m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL operator+=(const Quaternion &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_add_ps(m_data, other.m_data);
#else
      m_comps[0] += other.m_comps[0], m_comps[1] += other.m_comps[1],
          m_comps[2] += other.m_comps[2], m_comps[3] += other.m_comps[3];
#endif

      return *this;
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator-(const Quaternion &other) const
    {
#ifdef SF_USE_SIMD
      return Quaternion(_mm_sub_ps(m_data, other.m_data));
#else
      return Quaternion(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
          m_comps[2] - other.m_comps[2], m_comps[3] - other.m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL operator-=(const Quaternion &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_sub_ps(m_data, other.m_data);
#else
      m_comps[0] -= other.m_comps[0], m_comps[1] -= m_comps[1],
          m_comps[2] -= m_comps[2], m_comps[3] -= m_comps[3];
#endif

      return *this;
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator*(Real scalar) const
    {
#ifdef SF_USE_SIMD
      __m128 s = _mm_load_ss(&scalar);
      s = _mm_shuffle_ps(s, s, 0x0);

      return Quaternion(_mm_mul_ps(m_data, s));
#else
      return Quaternion(m_comps[0] * scalar, m_comps[1] * scalar,
          m_comps[2] * scalar, m_comps[3] * scalar);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL operator*=(Real scalar)
    {
#ifdef SF_USE_SIMD
      __m128 s = _mm_load_ss(&scalar);
      m_data = _mm_mul_ps(m_data, _mm_shuffle_ps(s, s, 0x0));
#else
      m_comps[0] *= scalar, m_comps[1] *= scalar, m_comps[2] *= scalar,
          m_comps[3] * scalar;
#endif

      return *this;
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator/(Real scalar) const
    {
#ifdef SF_USE_SIMD
      __m128 s = _mm_load_ss(&scalar);
      s = _mm_shuffle_ps(s, s, 0x0);

      return Quaternion(_mm_div_ps(m_data, s));
#else
      return Quaternion(m_comps[0] / scalar, m_comps[1] / scalar,
          m_comps[2] / scalar, m_comps[3] / scalar);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL operator/=(Real scalar)
    {
#ifdef SF_USE_SIMD
      __m128 s = _mm_load_ss(&scalar);
      m_data = _mm_mul_ps(m_data, _mm_shuffle_ps(s, s, 0x0));
#else
      m_comps[0] /= scalar, m_comps[1] /= scalar, m_comps[2] /= scalar,
          m_comps[3] /= scalar;
#endif

      return *this;
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator*(const Quaternion &other) const
    {
#ifdef SF_USE_SIMD
      __m128 l = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
      __m128 r = _mm_shuffle_ps(other.m_data, other.m_data, 0xE5); //_MM_SHUFFLE(3, 2, 1, 1)

      __m128 res = _mm_mul_ps(l, r);

      l = _mm_shuffle_ps(m_data, m_data, 0x66); //_MM_SHUFFLE(1, 2, 1, 2)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x42); //_MM_SHUFFLE(1, 0, 0, 2)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

      res = _mm_xor_ps(res, _mm_set_ps(-0.0f, +0.0f, +0.0f, +0.0f));
      l = _mm_mul_ps(l, r);
      l = _mm_xor_ps(l, _mm_set_ps(+0.0f, -0.0f, -0.0f, -0.0f));

      return Quaternion(_mm_add_ps(res, l));
#else
      return Quaternion(m_comps[0] * other.m_comps[0] - m_comps[1] * other.m_comps[1]
          - m_comps[2] * other.m_comps[2] - m_comps[3] * other.m_comps[3],
          m_comps[0] * other.m_comps[1] + m_comps[1] * other.m_comps[0] +
          m_comps[2] * other.m_comps[3] - m_comps[3] * other.m_comps[2],
          m_comps[0] * other.m_comps[2] - m_comps[1] * other.m_comps[3] +
          m_comps[2] * other.m_comps[0] + m_comps[3] * other.m_comps[1],
          m_comps[0] * other.m_comps[3] + m_comps[1] * other.m_comps[2] -
          m_comps[2] * other.m_comps[1] + m_comps[3] * other.m_comps[0]);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL operator*=(const Quaternion &other)
    {
#ifdef SF_USE_SIMD
      __m128 l = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
      __m128 r = _mm_shuffle_ps(other.m_data, other.m_data, 0xE5); //_MM_SHUFFLE(3, 2, 1, 1)

      __m128 res = _mm_mul_ps(l, r);

      l = _mm_shuffle_ps(m_data, m_data, 0x66); //_MM_SHUFFLE(1, 2, 1, 2)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x42); //_MM_SHUFFLE(1, 0, 0, 2)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

      res = _mm_xor_ps(res, _mm_set_ps(-0.0f, +0.0f, +0.0f, +0.0f));
      l = _mm_mul_ps(l, r);
      l = _mm_xor_ps(l, _mm_set_ps(+0.0f, -0.0f, -0.0f, -0.0f));

      m_data = _mm_add_ps(res, l);
#else
      Set(m_comps[0] * other.m_comps[0] - m_comps[1] * other.m_comps[1]
          - m_comps[2] * other.m_comps[2] - m_comps[3] * other.m_comps[3],
          m_comps[0] * other.m_comps[1] + m_comps[1] * other.m_comps[0] +
          m_comps[2] * other.m_comps[3] - m_comps[3] * other.m_comps[2],
          m_comps[0] * other.m_comps[2] - m_comps[1] * other.m_comps[3] +
          m_comps[2] * other.m_comps[0] + m_comps[3] * other.m_comps[1],
          m_comps[0] * other.m_comps[3] + m_comps[1] * other.m_comps[2] -
          m_comps[2] * other.m_comps[1] + m_comps[3] * other.m_comps[0]);
#endif

      return *this;
    }

    SF_FORCE_INLINE Real SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(i >= 0 && i < 4, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE const Real SF_VEC_CALL operator[](int i) const
    {
      SF_ASSERT(i >= 0 && i < 4, "Index is out of range");
      return m_comps[i];
    }
  private:
    //Scalar goes before the vector part
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
}
