/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector3.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   3D Vector

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
  SF_ATTR_ALIGN_16 class Vector3
  {
  public:
    static const Vector3 SF_ATTR_ALIGN_16 c_zero;
    static const Vector3 SF_ATTR_ALIGN_16 c_xAxis;
    static const Vector3 SF_ATTR_ALIGN_16 c_yAxis;
    static const Vector3 SF_ATTR_ALIGN_16 c_zAxis;

    SF_FORCE_INLINE SF_VEC_CALL Vector3(void)
    {

    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Vector3(Real x, Real y, Real z)
    {
  #ifdef SF_USE_SIMD
      m_data = _mm_set_ps(0.0, z, y, x);
  #else
      m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = 0.0;
  #endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator=(const Vector3 &other)
    {
#ifdef SF_USE_SIMD
      m_data = other.m_data;
#else
      m_comps[0] = other.m_comps[0];
      m_comps[1] = other.m_comps[1];
      m_comps[2] = other.m_comps[2];
#endif

      return *this;
    }

  #ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Vector3(__m128 data)
    {
      m_data = data;
    }

    SF_FORCE_INLINE __m128 SF_VEC_CALL Get128(void) const
    {
      return m_data;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(__m128 data)
    {
      m_data = data;
    }
  #endif

    SF_FORCE_INLINE Real SF_VEC_CALL GetX(void) const
    {
      return m_comps[0];
    }

    SF_FORCE_INLINE Real SF_VEC_CALL GetY(void) const
    {
      return m_comps[1];
    }

    SF_FORCE_INLINE Real SF_VEC_CALL GetZ(void) const
    {
      return m_comps[2];
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetX(Real x)
    {
      m_comps[0] = x;
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetY(Real y)
    {
      m_comps[1] = y;
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetZ(Real z)
    {
      m_comps[2] = z;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real x, Real y, Real z)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(0.0, z, y, x);
#else
      m_comps[0] = x, m_comps[1] = y, m_comps[2] = z;
#endif
    }

    SF_FORCE_INLINE void SF_VEC_CALL Splat(Real xyz)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set1_ps(xyz);
      m_comps[3] = Real(0.0);
#else
      m_comps[0] = m_comps[1] = m_comps[2] = xyz;
#endif
    }

    //Compute dot product of two vectors
    SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector3 &other) const
    {
  #ifdef SF_USE_SIMD
      __m128 d = _mm_mul_ps(m_data, other.m_data);
      __m128 z = _mm_shuffle_ps(d, d, 0xFE); //_MM_SHUFFLE(3, 3, 3, 2)
      __m128 y = _mm_shuffle_ps(d, d, 0XFD); //_MM_SHUFFLE(3, 3, 3, 1)

      d = _mm_add_ps(d, y);
      d = _mm_add_ps(d, z);

      return _mm_cvtss_f32(d);
  #else
      return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
          + m_comps[2] * other.m_comps[2];
  #endif
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL Cross(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      __m128 left = _mm_shuffle_ps(other.m_data, other.m_data, 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)
      left = _mm_mul_ps(left, m_data);

      __m128 right = _mm_shuffle_ps(m_data, m_data, 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)
      right = _mm_mul_ps(right, other.m_data);

      left = _mm_sub_ps(left, right);

      return Vector3(_mm_shuffle_ps(left, left, 0xC9));
#else
      return Vector3(m_comps[1] * other.m_comps[2] - m_comps[2] * other.m_comps[1],
          m_comps[2] * other.m_comps[0] - m_comps[0] * other.m_comps[2],
          m_comps[0] * other.m_comps[1] - m_comps[1] * other.m_comps[0]);
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

    //Nomralize the vector
    SF_FORCE_INLINE Vector3& SF_VEC_CALL Normalize(void)
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this /= Length();
    }

    //Return normalized copy of the vector
    SF_FORCE_INLINE Vector3 SF_VEC_CALL Normalized(void) const
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this / Length();
    }

    SF_FORCE_INLINE void SF_VEC_CALL ZeroOut(void)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_setzero_ps();
#else
      m_comps[0] = m_comps[1] = m_comps[2] = m_comps[3] = 0.0;
#endif
    }

    SF_FORCE_INLINE bool SF_VEC_CALL IsZeroEpsilon(void) const
    {
       return LengthSq() < SF_EPSILON * SF_EPSILON;
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

      return (m_comps[0] == inf) || (m_comps[1] == inf) || (m_comps[2] == inf);
  #endif
    }

    //Return a vector with absolute values of each component
    SF_FORCE_INLINE Vector3 SF_VEC_CALL GetAbs(void) const
    {
  #ifdef SF_USE_SIMD
      return Vector3(_mm_and_ps(m_data, c_SIMDAbsMask));
  #else
      return Vector3(MathUtils::Abs(m_comps[0]), MathUtils::Abs(m_comps[1]), 
        MathUtils::Abs(m_comps[2]));
  #endif
    }

    //Performs abs operation on the current vector
    SF_FORCE_INLINE Vector3& SF_VEC_CALL Abs(void)
    {
  #ifdef SF_USE_SIMD
      m_data = _mm_and_ps(m_data, c_SIMDAbsMask);
  #else
      m_comps[0] = MathUtils::Abs(m_comps[0]);
      m_comps[1] = MathUtils::Abs(m_comps[1]);
      m_comps[2] = MathUtils::Abs(m_comps[2]);
  #endif
      return *this;
    }

    //Returns a negated vector
    SF_FORCE_INLINE Vector3 SF_VEC_CALL Negated(void) const
    {
  #ifdef SF_USE_SIMD
      return Vector3(_mm_xor_ps(m_data, c_SIMDNegMask));
  #else
      return Vector3(-m_comps[0], -m_comps[1], -m_comps[2]);
  #endif
    }

    //Negates the current vector
    SF_FORCE_INLINE Vector3& SF_VEC_CALL Negate(void)
    {
  #ifdef SF_USE_SIMD
      m_data = _mm_xor_ps(m_data, c_SIMDNegMask);
  #else
      m_comps[0] = -m_comps[0];
      m_comps[1] = -m_comps[1];
      m_comps[2] = -m_comps[2];
  #endif
      return *this;
    }

    //Return distance between two points
    SF_FORCE_INLINE Real SF_VEC_CALL Distance(const Vector3 &other) const
    {
      return (*this - other).Length();
    }

    SF_FORCE_INLINE Real SF_VEC_CALL DistanceSq(const Vector3 &other) const
    {
      return (*this - other).LengthSq();
    }

    //Return the index of the max axis
    SF_FORCE_INLINE int SF_VEC_CALL MaxAxis(void) const
    {
      return m_comps[0] < m_comps[1] ? (m_comps[1] < m_comps[2] ? 2 : 1)
                                     : (m_comps[0] < m_comps[2] ? 2 : 0);
    }

    //Return the index of the min axis
    SF_FORCE_INLINE int SF_VEC_CALL MinAxis(void) const
    {
      return m_comps[0] < m_comps[1] ? (m_comps[0] < m_comps[2] ? 0 : 2)
                                     : (m_comps[1] < m_comps[2] ? 1 : 2);
    }

    //Return the value of the max axis
    SF_FORCE_INLINE Real MaxAxisValue(void) const
    {
      return m_comps[0] < m_comps[1]
              ? (m_comps[1] < m_comps[2] ? m_comps[2] : m_comps[1])
              : (m_comps[0] < m_comps[2] ? m_comps[2] : m_comps[0]);
    }

    //Return the value of the min axis
    SF_FORCE_INLINE Real SF_VEC_CALL MinAxisValue(void) const
    {
      return m_comps[0] < m_comps[1]
              ? (m_comps[0] < m_comps[2] ? m_comps[0] : m_comps[2])
              : (m_comps[1] < m_comps[2] ? m_comps[1] : m_comps[2]);
    }

    //Linear interpolation
    SF_FORCE_INLINE Vector3 SF_VEC_CALL Lerp(const Vector3 &other, Real t) const
    {
#ifdef SF_USE_SIMD
      __m128 mt = _mm_load_ss(&t);
      mt = _mm_shuffle_ps(mt, mt, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0);

      __m128 lerpDir = _mm_sub_ps(other.m_data, m_data);
      __m128 lerpMove = _mm_mul_ps(lerpDir, mt);

      return Vector3(_mm_add_ps(m_data, lerpMove));
#else
      return (*this) + (other - *this) * t;
#endif
    }

    //Get rotated vector around arbitrary axis. Axis has to be normalized
    SF_FORCE_INLINE Vector3 SF_VEC_CALL RotatedRad(const Vector3 &axis,
                                                   Real angleRad) const
    {
#ifdef SF_USE_SIMD
      Real sinVal = MathUtils::Sin(angleRad);
      Real cosVal = MathUtils::Cos(angleRad);

      __m128 mSin = _mm_load_ss(&sinVal);
      mSin = _mm_shuffle_ps(mSin, mSin, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0)

      __m128 mCos = _mm_load_ss(&cosVal);
      mCos = _mm_shuffle_ps(mCos, mCos, 0xC0); //_MM_SHUFFLE(3, 0, 0, 0)

      __m128 p = _mm_mul_ps(m_data, axis.m_data);
      __m128 py = _mm_shuffle_ps(p, p, 0xD2); //(z, x, y, 0) _MM_SHUFFLE(3, 1, 0, 2)
      __m128 pz = _mm_shuffle_ps(p, p, 0xC9); //(y, z, x, 0) _MM_SHUFFLE(3, 0, 2, 1)

      p = _mm_add_ps(p, py);
      p = _mm_add_ps(p, pz);
      p = _mm_mul_ps(axis.m_data, p);

      __m128 n = axis.Cross(*this).Get128();

      __m128 d = _mm_sub_ps(m_data, p);

      n = _mm_mul_ps(n, mSin);
      d = _mm_mul_ps(d, mCos);

      p = _mm_add_ps(d, p);
      return Vector3(_mm_add_ps(p, n));
#else
      Vector3 p = axis * this->Dot(axis);
      Vector3 n = axis.Cross(*this);
      Vector3 d = *this - p;

      return d * MathUtils::Cos(angleRad) + n * MathUtils::Sin(angleRad) + p;
#endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL RotateRad(const Vector3 &axis,
                                                   Real angleRad)
    {
#ifdef SF_USE_SIMD
      m_data = this->RotatedRad(axis, angleRad).m_data;
#else
      *this = this->RotatedRad(axis, angleRad);
#endif

      return *this;
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL RotatedDeg(const Vector3 &axis,
                                                   Real angleDeg) const
    {
      return this->RotatedRad(axis, angleDeg * SF_RADS_PER_DEG);
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL RotateDeg(const Vector3 &axis,
                                                   Real angleDeg)
    {
      return this->RotateRad(axis, angleDeg * SF_RADS_PER_DEG);
    }

    //Get angle between two vectors
    SF_FORCE_INLINE Real SF_VEC_CALL GetAngle(const Vector3 &other) const
    {
      Real scalar = MathUtils::Sqrt(this->LengthSq() * other.LengthSq());

      SF_ASSERT(scalar != 0, "One of the vectors has length 0");

      return MathUtils::Acos(this->Dot(other) / scalar);
    }

    //Addition operators
    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator+(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector3(_mm_add_ps(m_data, other.m_data));
#else
      return Vector3(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
          m_comps[2] + other.m_comps[2]);
#endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator+=(const Vector3 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_add_ps(m_data, other.m_data);
#else
      m_comps[0] += other.m_comps[0];
      m_comps[1] += other.m_comps[1];
      m_comps[2] += other.m_comps[2];
#endif

      return *this;
    }

    //Negation operator
    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator-(void) const
    {
#ifdef SF_USE_SIMD
      return Vector3(_mm_xor_ps(m_data, c_SIMDNegMask));
#else
      return Vector3(-m_comps[0], -m_comps[1], -m_comps[2]);
#endif
    }

    //Subtraction operators
    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator-(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector3(_mm_sub_ps(m_data, other.m_data));
#else
      return Vector3(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
          m_comps[2] - other.m_comps[2]);
#endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator-=(const Vector3 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_sub_ps(m_data, other.m_data);
#else
      m_comps[0] -= other.m_comps[0];
      m_comps[1] -= other.m_comps[1];
      m_comps[2] -= other.m_comps[2];
#endif

      return *this;
    }

    //Scaling operators
    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator*(Real scalar) const
    {
#ifdef SF_USE_SIMD
      return Vector3(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector3(m_comps[0] * scalar, m_comps[1] * scalar,
          m_comps[2] * scalar);
#endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator*=(Real scalar)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] *= scalar;
      m_comps[1] *= scalar;
      m_comps[2] *= scalar;
#endif

      return *this;
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator*(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector3(_mm_mul_ps(m_data, other.Get128()));
#else
      return Vector3(m_comps[0] * other.m_comps[0], m_comps[1] * other.m_comps[1],
        m_comps[2] * other.m_comps[2]);
#endif
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator/(const Vector3 &other) const
    {
      SF_ASSERT(other.m_comps[0] == 0.0f 
        || other.m_comps[1] == 0.0f || other.m_comps[2] == 0.0f, "Division by zero");
#ifdef SF_USE_SIMD
      return Vector3(_mm_div_ps(m_data, other.Get128()));
#else
      return Vector3(m_comps[0] / other.m_comps[0], m_comps[1] / other.m_comps[1],
        m_comps[2] / other.m_comps[2]);
#endif
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator/(Real scalar) const
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");
#ifdef SF_USE_SIMD
      return Vector3(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector3(m_comps[0] / scalar, m_comps[1] / scalar,
        m_comps[2] / scalar);
#endif
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator/=(Real scalar)
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");
#ifdef SF_USE_SIMD
      m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] /= scalar;
      m_comps[1] /= scalar;
      m_comps[2] /= scalar;
#endif

      return *this;
    }

    //Index access
    SF_FORCE_INLINE Real& SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(i >= 0 && i <= 2, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE const Real& SF_VEC_CALL operator[](int i) const
    {
      SF_ASSERT(i >= 0 && i <= 2, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator==(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
      return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1])
          && (m_comps[2] == other.m_comps[2]);
#endif
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator!=(const Vector3 &other) const
    {
      return !(*this == other);
    }

  private:
#ifdef SF_USE_SIMD
    union
    {
      Real   m_comps[4];
      __m128 m_data;
    };
#else
    Real m_comps[4];
#endif
  };

  SF_FORCE_INLINE Vector3 SF_VEC_CALL operator*(Real scalar, const Vector3 &v)
  {
#ifdef SF_USE_SIMD
    return Vector3(_mm_mul_ps(v.Get128(), _mm_set1_ps(scalar)));
#else
    return Vector3(v.GetX() * scalar, v.GetY() * scalar,
      v.GetZ() * scalar);
#endif
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector3 &v1, const Vector3 &v2)
  {
    return v1.Dot(v2);
  }

  SF_FORCE_INLINE Vector3 SF_VEC_CALL Cross(const Vector3 &v1, const Vector3 &v2)
  {
    return v1.Cross(v2);
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Length(const Vector3 &v)
  {
    return v.Length();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(const Vector3 &v)
  {
    return v.LengthSq();
  }

  SF_FORCE_INLINE Vector3& SF_VEC_CALL Normalize(Vector3 &v)
  {
    return v.Normalize();
  }

  SF_FORCE_INLINE Vector3 SF_VEC_CALL Normalized(const Vector3 &v)
  {
    return v.Normalized();
  }

  SF_FORCE_INLINE Vector3& SF_VEC_CALL Abs(Vector3 &v)
  {
    return v.Abs();
  }

  SF_FORCE_INLINE Vector3& SF_VEC_CALL Negate(Vector3 &v)
  {
    return v.Negate();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Distance(const Vector3 &v1, const Vector3 &v2)
  {
    return v1.Distance(v2);
  }

  SF_FORCE_INLINE Real SF_VEC_CALL DistanceSq(const Vector3 &v1, const Vector3 &v2)
  {
    return v1.DistanceSq(v2);
  }

  SF_FORCE_INLINE int SF_VEC_CALL MaxAxis(const Vector3 &v)
  {
    return v.MaxAxis();
  }

  SF_FORCE_INLINE int SF_VEC_CALL MinAxis(const Vector3 &v)
  {
    return v.MinAxis();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL MaxAxisValue(const Vector3 &v)
  {
    return v.MaxAxisValue();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL MinAxisValue(const Vector3 &v)
  {
    return v.MinAxisValue();
  }

  SF_FORCE_INLINE Vector3 SF_VEC_CALL Lerp(const Vector3 &start,
                                           const Vector3 &end, Real t)
  {
    return start.Lerp(end, t);
  }

  SF_FORCE_INLINE Vector3& SF_VEC_CALL RotateRad(Vector3 &v, const Vector3 &axis,
                                                 Real angleRad)
  {
    return v.RotateRad(axis, angleRad);
  }

  SF_FORCE_INLINE Vector3& SF_VEC_CALL RotateDeg(Vector3 &v, const Vector3 &axis,
                                                 Real angleDeg)
  {
    return v.RotateDeg(axis, angleDeg);
  }

  SF_FORCE_INLINE void SF_VEC_CALL GramSchmidt(Vector3 &e1,
    Vector3 &e2, Vector3 &e3)
  {
    Real absX = MathUtils::Abs(e1[0]);
    if (absX > MathUtils::Abs(e1[1]) &&
      absX > MathUtils::Abs(e1[2]))
    {
      e2[0] = -e1[1];
      e2[1] = e1[0];
      e2[2] = Real(0.0);
    }
    else
    {
      e2[0] = Real(0.0);
      e2[1] = e1[2];
      e2[2] = -e1[1];
    }

    e1.Normalize();
    e2.Normalize();
    e3 = e1.Cross(e2);
    e3.Normalize();

    SF_ASSERT(Dot(e1, e2) == 0, "Basis is not orthogonal");
    SF_ASSERT(Dot(e1, e3) == 0, "Basis is not orthogonal");
  }

}
