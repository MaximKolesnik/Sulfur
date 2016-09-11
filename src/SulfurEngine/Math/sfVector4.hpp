/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector4.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   4D Vector

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../sfProjectDefs.hpp"
#include "sfMathDefs.hpp"
#include "sfVector3.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  SF_ATTR_ALIGN_16 class Vector4
  {
  public:
    static const Vector4 SF_ATTR_ALIGN_16 c_zero;
    static const Vector4 SF_ATTR_ALIGN_16 c_xAxis;
    static const Vector4 SF_ATTR_ALIGN_16 c_yAxis;
    static const Vector4 SF_ATTR_ALIGN_16 c_zAxis;
    static const Vector4 SF_ATTR_ALIGN_16 c_wAxis;

    SF_FORCE_INLINE SF_VEC_CALL Vector4(void)
    {

    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Vector4(Real x, Real y, Real z, Real w)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(w, z, y, x);
#else
      m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = w;
#endif
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator=(const Vector4 &other)
    {
#ifdef SF_USE_SIMD
      m_data = other.m_data;
#else
      m_comps[0] = other.m_comps[0];
      m_comps[1] = other.m_comps[1];
      m_comps[2] = other.m_comps[2];
      m_comps[3] = other.m_comps[3];
#endif
      return *this;
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Vector4(__m128 data)
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

    SF_FORCE_INLINE Real SF_VEC_CALL GetW(void) const
    {
      return m_comps[3];
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

    SF_FORCE_INLINE void SF_VEC_CALL SetW(Real w)
    {
      m_comps[3] = w;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real x, Real y, Real z, Real w)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_set_ps(w, z, y, x);
#else
      m_comps[0] = x, m_comps[1] = y, m_comps[2] = z, m_comps[3] = w;
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector4 &other) const
    {
#ifdef SF_USE_SIMD
      __m128 m = _mm_mul_ps(m_data, other.m_data);
      __m128 w = _mm_shuffle_ps(m, m, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
      __m128 z = _mm_shuffle_ps(m, m, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
      __m128 y = _mm_shuffle_ps(m, m, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

      m = _mm_add_ps(m, w);
      m = _mm_add_ps(m, z);
      m = _mm_add_ps(m, y);

      return _mm_cvtss_f32(m);
#else
      return m_comps[0] * other.m_comps[0] + m_comps[1] * other.m_comps[1]
        + m_comps[2] * other.m_comps[2] + m_comps[3] * other.m_comps[3];
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector3 &other) const
    {
#ifdef SF_USE_SIMD
      __m128 v = _mm_set_ps(1.0, other[2], other[1], other[0]);
      __m128 m = _mm_mul_ps(m_data, v);
      __m128 w = _mm_shuffle_ps(m, m, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
      __m128 z = _mm_shuffle_ps(m, m, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
      __m128 y = _mm_shuffle_ps(m, m, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

      m = _mm_add_ps(m, w);
      m = _mm_add_ps(m, z);
      m = _mm_add_ps(m, y);

      return _mm_cvtss_f32(m);
#else
      return m_comps[0] * other[0] + m_comps[1] * other[1]
        + m_comps[2] * other[2] + m_comps[3];
#endif
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Length(void) const
    {
      return MathUtils::Sqrt(this->Dot(*this));
    }

    SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(void) const
    {
      return this->Dot(*this);
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL Normalize(void)
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this /= Length();
    }

    SF_FORCE_INLINE Vector4 SF_VEC_CALL Normalized(void) const
    {
      SF_ASSERT(!IsZeroEpsilon(), "Trying to normalize vector of length 0");
      return *this / Length();
    }

    SF_FORCE_INLINE bool SF_VEC_CALL IsZeroEpsilon(void) const
    {
      return LengthSq() != SF_EPSILON * SF_EPSILON;
    }

    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator+(const Vector4 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector4(_mm_add_ps(m_data, other.m_data));
#else
      return Vector4(m_comps[0] + other.m_comps[0], m_comps[1] + other.m_comps[1],
        m_comps[2] + other.m_comps[2], m_comps[3] + other.m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator+=(const Vector4 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_add_ps(m_data, other.m_data);
#else
      m_comps[0] += other.m_comps[0];
      m_comps[1] += other.m_comps[1];
      m_comps[2] += other.m_comps[2];
      m_comps[3] += other.m_comps[3];
#endif

      return *this;
    }

    //Negation operator
    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator-(void) const
    {
#ifdef SF_USE_SIMD
      return Vector4(_mm_xor_ps(m_data, c_SIMDNegMask));
#else
      return Vector4(-m_comps[0], -m_comps[1], -m_comps[2], -m_comps[3]);
#endif
    }

    //Subtraction operators
    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator-(const Vector4 &other) const
    {
#ifdef SF_USE_SIMD
      return Vector4(_mm_sub_ps(m_data, other.m_data));
#else
      return Vector4(m_comps[0] - other.m_comps[0], m_comps[1] - other.m_comps[1],
        m_comps[2] - other.m_comps[2], m_comps[3] - other.m_comps[3]);
#endif
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator-=(const Vector4 &other)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_sub_ps(m_data, other.m_data);
#else
      m_comps[0] -= other.m_comps[0];
      m_comps[1] -= other.m_comps[1];
      m_comps[2] -= other.m_comps[2];
      m_comps[3] -= other.m_comps[3];
#endif

      return *this;
    }

    //Scaling operators
    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator*(Real scalar) const
    {
#ifdef SF_USE_SIMD
      return Vector4(_mm_mul_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector4(m_comps[0] * scalar, m_comps[1] * scalar,
        m_comps[2] * scalar, m_comps[3] * scalar);
#endif
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator*=(Real scalar)
    {
#ifdef SF_USE_SIMD
      m_data = _mm_mul_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] *= scalar;
      m_comps[1] *= scalar;
      m_comps[2] *= scalar;
      m_comps[3] *= scalar;
#endif

      return *this;
    }

    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator/(Real scalar) const
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");
#ifdef SF_USE_SIMD
      return Vector4(_mm_div_ps(m_data, _mm_set1_ps(scalar)));
#else
      return Vector4(m_comps[0] / scalar, m_comps[1] / scalar,
        m_comps[2] / scalar, m_comps[3] / scalar);
#endif
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator/=(Real scalar)
    {
      SF_ASSERT(scalar != Real(0.0), "Scaling by 1/0");
#ifdef SF_USE_SIMD
      m_data = _mm_div_ps(m_data, _mm_set1_ps(scalar));
#else
      m_comps[0] /= scalar;
      m_comps[1] /= scalar;
      m_comps[2] /= scalar;
      m_comps[3] /= scalar;
#endif

      return *this;
    }

    //Index access
    SF_FORCE_INLINE Real& SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(i >= 0 && i <= 3, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE const Real& SF_VEC_CALL operator[](int i) const
    {
      SF_ASSERT(i >= 0 && i <= 3, "Index is out of range");
      return m_comps[i];
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator==(const Vector4 &other) const
    {
#ifdef SF_USE_SIMD
      return 0xF == _mm_movemask_ps(_mm_cmp_ps(m_data, other.m_data, _CMP_EQ_OQ));
#else
      return (m_comps[0] == other.m_comps[0]) && (m_comps[1] == other.m_comps[1])
        && (m_comps[2] == other.m_comps[2]) && (m_comps[3] == other.m_comps[3]);
#endif
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator!=(const Vector4 &other) const
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

  SF_FORCE_INLINE Vector4 SF_VEC_CALL operator*(Real scalar, const Vector4 &v)
  {
#ifdef SF_USE_SIMD
    return Vector4(_mm_mul_ps(v.Get128(), _mm_set1_ps(scalar)));
#else
    return Vector4(v.GetX() * scalar, v.GetY() * scalar,
      v.GetZ() * scalar, v.GetZ() * scalar);
#endif
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Dot(const Vector4 &v1, const Vector4 &v2)
  {
    return v1.Dot(v2);
  }

  SF_FORCE_INLINE Vector4& SF_VEC_CALL Normalize(Vector4 &v)
  {
    return v.Normalize();
  }

  SF_FORCE_INLINE Vector4& SF_VEC_CALL Normalized(const Vector4 &v)
  {
    return v.Normalized();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL Length(const Vector4 &v)
  {
    return v.Length();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL LengthSq(const Vector4 &v)
  {
    return v.LengthSq();
  }
}