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
  class Matrix4;
  class Matrix3;

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
      Set(axis, angleRad);
    }

    explicit SF_VEC_CALL Quaternion(const Matrix3 &m);
    explicit SF_VEC_CALL Quaternion(const Matrix4 &m);

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

    Matrix3 SF_VEC_CALL GetMatrix3(void) const;
    Matrix4 SF_VEC_CALL GetMatrix4(void) const;

    SF_FORCE_INLINE void SF_VEC_CALL Set(const Vector3 &axis, Real angleRad)
    {
      Real axisLength = axis.Length();

      SF_ASSERT(axisLength != 0.0, "Axis has length of 0");

      Real scalar = MathUtils::Sin(angleRad * Real(0.5)) / axisLength;

      Set(MathUtils::Cos(angleRad * Real(0.5)), axis.GetX() * scalar, axis.GetY() * scalar,
          axis.GetZ() * scalar);
    }

    void SF_VEC_CALL Set(const Matrix3 &m);
    void SF_VEC_CALL Set(const Matrix4 &m);

    //XZY
    SF_FORCE_INLINE void SF_VEC_CALL SetEuler(Real roll, Real pitch, Real yaw)
    {
      Real cx = MathUtils::Cos(yaw * Real(0.5));  //heading
      Real cy = MathUtils::Cos(pitch * Real(0.5));//attitude
      Real cz = MathUtils::Cos(roll * Real(0.5)); //bank
      Real sx = MathUtils::Sin(yaw * Real(0.5));
      Real sy = MathUtils::Sin(pitch * Real(0.5));
      Real sz = MathUtils::Sin(roll * Real(0.5));

#ifdef SF_USE_SIMD
      __m128 q1 = _mm_set1_ps(cz);
      __m128 q2 = _mm_set1_ps(sz);
      
      q2 = _mm_xor_ps(q2, _mm_castsi128_ps(_mm_set_epi32(0x80000000, 0x00000000, 
        0x00000000, 0x80000000)));

      __m128 s = _mm_set_ps(sy, cy, sy, cy);
      q1 = _mm_mul_ps(q1, s);

      s = _mm_shuffle_ps(s, s, 0x11); //_MM_SHUFFLE(0, 1, 0, 1)
      q2 = _mm_mul_ps(q2, s);

      s = _mm_set_ps(cx, sx, sx, cx);
      q1 = _mm_mul_ps(q1, s);

      s = _mm_shuffle_ps(s, s, 0x41); //_MM_SHUFFLE(1, 0, 0, 1)
      q2 = _mm_mul_ps(q2, s);

      m_data = _mm_add_ps(q1, q2);
#else
#endif
      m_comps[0] = cx * cy * cz - sx * sy * sz;
      m_comps[1] = sx * sy * cz + cx * cy * sz;
      m_comps[2] = sx * cy * cz + cx * sy * sz;
      m_comps[3] = cx * sy * cz - sx * cy * sz;
    }

    SF_FORCE_INLINE void SF_VEC_CALL GetEulerXYZ(Real &roll, Real &pitch, Real &yaw)
    {
      Real test = m_comps[1] * m_comps[2] + m_comps[3] * m_comps[0];
      if (test > 0.499) 
      {
        yaw = 2 * MathUtils::Atan2(m_comps[1], m_comps[0]);
        pitch = SF_PI / Real(2.0);
        roll = 0;
        return;
      }
      if (test < -0.499) 
      {
        yaw = -2 * MathUtils::Atan2(m_comps[1], m_comps[0]);
        pitch = -SF_PI / Real(2.0);
        roll = 0;
        return;
      }

      Real sqx = m_comps[1] * m_comps[1];
      Real sqy = m_comps[2] * m_comps[2];
      Real sqz = m_comps[3] * m_comps[3];
      yaw = atan2(2 * m_comps[2] * m_comps[0] - 2 * m_comps[1] * m_comps[3], 
        1 - 2 * sqy - 2 * sqz);

      pitch = asin(2 * test);

      roll = atan2(2 * m_comps[1] * m_comps[0] - 2 * m_comps[2] * m_comps[3], 
        1 - 2 * sqx - 2 * sqz);
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real q0, Real q1, Real q2, Real q3)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(q3, q2, q1, q0);
#else
      m_comps[0] = q0, m_comps[1] = q1, m_comps[2] = q2, m_comps[3] = q3;
#endif
    }

    SF_FORCE_INLINE void SF_VEC_CALL GetAxisAngle(Vector3 &axis, Real &angle) const
    {
      //0 angle rotation, pick any axis
      if (m_comps[0] == Real(1.0))
      {
        angle = Real(0.0);
        axis = Vector3::c_xAxis;
        return;
      }
      //180 angle rotation
      else if (m_comps[0] == Real(0.0))
      {
        angle = SF_PI;
        axis[0] = m_comps[1];
        axis[1] = m_comps[2];
        axis[2] = m_comps[3];
        return;
      }

      //No singularities
#ifdef SF_USE_SIMD
      __m128 s = _mm_set1_ps(m_comps[0]);
      s = _mm_mul_ps(s, s);
      s = _mm_sub_ps(_mm_set1_ps(1), s);
      s = _mm_rsqrt_ps(s);
      
      __m128 vec = _mm_set_ps(0.0, m_comps[3], m_comps[2], m_comps[1]);
      vec = _mm_mul_ps(s, vec);

      __m128 mAngle = _mm_set1_ps(MathUtils::Acos(m_comps[0]));
      mAngle = _mm_mul_ps(mAngle, _mm_set1_ps(2));

      angle = _mm_cvtss_f32(mAngle);
      axis.Set(vec);
#else
      angle = 2 * MathUtils::Acos(m_comps[0]);
      Real s = Real(1.0) / MathUtils::Sqrt(1 - m_comps[0] * m_comps[0]);

      axis[0] = m_comps[1] * s;
      axis[1] = m_comps[2] * s;
      axis[2] = m_comps[3] * s;
#endif
    }

    //Invert the quaternion
    SF_FORCE_INLINE Quaternion& SF_VEC_CALL Invert(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_xor_ps(m_data, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));
#else
      m_comps[1] = -m_comps[1], m_comps[2] = -m_comps[2], m_comps[3] = -m_comps[3];
#endif

      return *this;
    }

    //Return inverted copy of the quaternion
    SF_FORCE_INLINE Quaternion SF_VEC_CALL Inverted(void) const
    {
#ifdef SF_USE_SIMD
      return Quaternion(_mm_xor_ps(m_data, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f)));
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
      __m128 length = _mm_add_ps(qMul,
        _mm_shuffle_ps(qMul, qMul, 0x1B)); //_MM_SHUFFLE(0, 1, 2, 3)
      length = _mm_add_ps(length, 
        _mm_shuffle_ps(qMul, qMul, 0x7E)); //_MM_SHUFFLE(1, 3, 3, 2)
      length = _mm_add_ps(length,
        _mm_shuffle_ps(qMul, qMul, 0x81)); //_MM_SHUFFLE(2, 0, 0, 1)

      length = _mm_sqrt_ps(length);
      length = _mm_rcp_ps(length);
      
      return Quaternion(_mm_mul_ps(length, m_data));
#else
      Real length = Real(1.0) / MathUtils::Sqrt(m_comps[0] * m_comps[0] 
        + m_comps[1] * m_comps[1] + m_comps[2] * m_comps[2] 
        + m_comps[3] * m_comps[3]);

      return Quaternion(m_comps[0] * length, m_comps[1] * length,
        m_comps[2] * length, m_comps[3] * length);
#endif
    }

    SF_FORCE_INLINE Quaternion& SF_VEC_CALL Normalize(void)
    {
      SF_ASSERT(this->Length() != 0, "Normalization of zero length Quaternion");
#ifdef SF_USE_SIMD
      __m128 qMul = _mm_mul_ps(m_data, m_data);
      __m128 length = _mm_add_ps(qMul,
        _mm_shuffle_ps(qMul, qMul, 0x1B)); //_MM_SHUFFLE(0, 1, 2, 3)
      length = _mm_add_ps(length,
        _mm_shuffle_ps(qMul, qMul, 0x7E)); //_MM_SHUFFLE(1, 3, 3, 2)
      length = _mm_add_ps(length,
        _mm_shuffle_ps(qMul, qMul, 0x81)); //_MM_SHUFFLE(2, 0, 0, 1)

      length = _mm_sqrt_ps(length);
      length = _mm_rcp_ps(length);

      m_data = _mm_mul_ps(length, m_data);
#else
      Real length = Real(1.0) / MathUtils::Sqrt(m_comps[0] * m_comps[0]
        + m_comps[1] * m_comps[1] + m_comps[2] * m_comps[2]
        + m_comps[3] * m_comps[3]);

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
      m_comps[0] -= other.m_comps[0], m_comps[1] -= other.m_comps[1],
          m_comps[2] -= other.m_comps[2], m_comps[3] -= other.m_comps[3];
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
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x82); //_MM_SHUFFLE(2, 0, 0, 2)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

      res = _mm_xor_ps(res, _mm_set_ps(+0.0f, +0.0f, +0.0f, -0.0f));
      l = _mm_mul_ps(l, r);
      l = _mm_xor_ps(l, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));

      return Quaternion(_mm_add_ps(res, l));
#else
      return Quaternion(
        m_comps[0] * other.m_comps[0] - m_comps[1] * other.m_comps[1]
       -m_comps[2] * other.m_comps[2] - m_comps[3] * other.m_comps[3],

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
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x82); //_MM_SHUFFLE(2, 0, 0, 2)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0xFB); //_MM_SHUFFLE(3, 3, 2, 3)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      res = _mm_add_ps(res, _mm_mul_ps(l, r));

      l = _mm_shuffle_ps(m_data, m_data, 0x9C); //_MM_SHUFFLE(2, 1, 3, 0)
      r = _mm_shuffle_ps(other.m_data, other.m_data, 0x78); //_MM_SHUFFLE(1, 3, 2, 0)

      res = _mm_xor_ps(res, _mm_set_ps(+0.0f, +0.0f, +0.0f, -0.0f));
      l = _mm_mul_ps(l, r);
      l = _mm_xor_ps(l, _mm_set_ps(-0.0f, -0.0f, -0.0f, +0.0f));

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

    SF_FORCE_INLINE Quaternion SF_VEC_CALL operator*(const Vector3 &v) const
    {
#ifdef SF_USE_SIMD
      __m128 q = _mm_shuffle_ps(m_data, m_data, 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
      __m128 mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x90); //_MM_SHUFFLE(2, 1, 0, 0)

      __m128 result = _mm_mul_ps(q, mv);

      q = _mm_shuffle_ps(m_data, m_data, 0x7A); //_MM_SHUFFLE(1, 3, 2, 2)
      mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x49); //_MM_SHUFFLE(1, 0, 2, 1)

      result = _mm_add_ps(result, _mm_mul_ps(q, mv));

      q = _mm_shuffle_ps(m_data, m_data, 0x9F); //_MM_SHUFFLE(2, 1, 3, 3)
      mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x26); //_MM_SHUFFLE(0, 2, 1, 2)

      q = _mm_mul_ps(q, mv);


      result = _mm_xor_ps(result, _mm_castsi128_ps(_mm_set_epi32(0x00000000, 0x00000000,
        0x00000000, 0x80000000)));
      q = _mm_xor_ps(q, c_SIMDNegMask);

      return Quaternion( _mm_add_ps(result, q));
#else
      return Quaternion(-m_comps[1] * v[0] - m_comps[2] * v[1] - m_comps[3] * v[2],
                         m_comps[0] * v[0] + m_comps[2] * v[2] - m_comps[3] * v[1],
                         m_comps[0] * v[1] + m_comps[3] * v[0] - m_comps[1] * v[2],
                         m_comps[0] * v[2] + m_comps[1] * v[1] - m_comps[2] * v[0]);
#endif
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL Slerp(const Quaternion &q, Real t) const
    {
      Quaternion slerped;

      Real cosHalfTheta = m_comps[0] * q[0] + m_comps[1] * q[1]
        + m_comps[2] * q[2] + m_comps[3] * q[3];

      if (MathUtils::Abs(cosHalfTheta) >= Real(1.0)) 
      {
        slerped[0] = m_comps[0];
        slerped[1] = m_comps[1];
        slerped[2] = m_comps[2];
        slerped[3] = m_comps[3];
        return slerped;
      }

      Real halfTheta = MathUtils::Acos(cosHalfTheta);
      Real sinHalfTheta = MathUtils::Sqrt(Real(1.0) - cosHalfTheta * cosHalfTheta);

      if (MathUtils::Abs(sinHalfTheta) < Real(0.001)) 
      {
        slerped.Set(Real(0.5) * (m_comps[0] + q[0]),
          Real(0.5) * (m_comps[1] + q[1]),
          Real(0.5) * (m_comps[2] + q[2]),
          Real(0.5) * (m_comps[3] + q[3]));

        return slerped;
      }

      Real ratioA = MathUtils::Sin((1 - t) * halfTheta) / sinHalfTheta;
      Real ratioB = MathUtils::Sin(t * halfTheta) / sinHalfTheta;

      return *this * ratioA + q * ratioB;
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL Rotated(const Vector3 &v) const
    {
      Quaternion q = *this * v * Inverted();

      return Vector3(q[1], q[2], q[3]);
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL Rotate(Vector3 &v) const
    {
      Quaternion q = *this * v * Inverted();

      v.Set(q[1], q[2], q[3]);

      return v;
    }

    SF_FORCE_INLINE Real& SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(i >= 0 && i < 4, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE const Real& SF_VEC_CALL operator[](int i) const
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

  SF_FORCE_INLINE Quaternion SF_VEC_CALL operator*(const Vector3 &v,
    const Quaternion &q)
  {
#ifdef SF_USE_SIMD
    __m128 mq = _mm_shuffle_ps(q.Get128(), q.Get128(), 0x1); //_MM_SHUFFLE(0, 0, 0, 1)
    __m128 mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x90); //_MM_SHUFFLE(2, 1, 0, 0)

    __m128 result = _mm_mul_ps(mq, mv);

    mq = _mm_shuffle_ps(q.Get128(), q.Get128(), 0x9E); //_MM_SHUFFLE(2, 1, 3, 2)
    mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x25); //_MM_SHUFFLE(0, 2, 1, 1)

    result = _mm_add_ps(result, _mm_mul_ps(mq, mv));

    mq = _mm_shuffle_ps(q.Get128(), q.Get128(), 0x7B); //_MM_SHUFFLE(1, 3, 2, 3)
    mv = _mm_shuffle_ps(v.Get128(), v.Get128(), 0x4A); //_MM_SHUFFLE(1, 0, 2, 2)

    mq = _mm_mul_ps(mq, mv);


    result = _mm_xor_ps(result, _mm_castsi128_ps(_mm_set_epi32(0x00000000, 0x00000000,
      0x00000000, 0x80000000)));
    mq = _mm_xor_ps(mq, c_SIMDNegMask);

    return Quaternion(_mm_add_ps(result, mq));
#else
    return Quaternion(-q[1] * v[0] - q[2] * v[1] - q[3] * v[2],
                       q[0] * v[0] + q[3] * v[1] - q[2] * v[2],
                       q[0] * v[1] + q[1] * v[2] - q[3] * v[0],
                       q[0] * v[2] + q[2] * v[0] - q[1] * v[1]);
#endif
  }

  SF_FORCE_INLINE Quaternion SF_VEC_CALL Slerp(const Quaternion &q1,
    const Quaternion &q2, Real t)
  {
    return q1.Slerp(q2, t);
  }
}
