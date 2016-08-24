// TODO: Add Quaternions

#pragma once

#include "../sfProjectDefs.hpp"
#include "sfMathDefs.hpp"
#include "sfVector3.hpp"
#include "sfQuaternion.hpp"

namespace Sulfur
{
  SF_ATTR_ALIGN_16 class Matrix3
  {
  public:
    static const Matrix3 SF_ATTR_ALIGN_16 c_identity;

    SF_FORCE_INLINE SF_VEC_CALL Matrix3(void)
    {
    }

    SF_FORCE_INLINE SF_VEC_CALL Matrix3(const Matrix3 &other)
    {
      m_rows[0] = other.m_rows[0];
      m_rows[1] = other.m_rows[1];
      m_rows[2] = other.m_rows[2];
    }

    SF_FORCE_INLINE SF_VEC_CALL Matrix3(Real e00, Real e01, Real e02,
                                        Real e10, Real e11, Real e12,
                                        Real e20, Real e21, Real e22)
    {
      m_rows[0].Set(e00, e01, e02);
      m_rows[1].Set(e10, e11, e12);
      m_rows[2].Set(e20, e21, e22);
    }

    SF_FORCE_INLINE SF_VEC_CALL Matrix3(const Vector3 &v0, const Vector3 &v1,
      const Vector3 &v2)
    {
      m_rows[0] = v0;
      m_rows[1] = v1;
      m_rows[2] = v2;
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Matrix3(__m128 v0, __m128 v1, __m128 v2)
    {
      m_rows[0] = Vector3(v0);
      m_rows[1] = Vector3(v1);
      m_rows[2] = Vector3(v2);
    }
#endif

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL operator=(const Matrix3 &other)
    {
      m_rows[0] = other.m_rows[0];
      m_rows[1] = other.m_rows[1];
      m_rows[2] = other.m_rows[2];

      return *this;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real e00, Real e01, Real e02,
      Real e10, Real e11, Real e12,
      Real e20, Real e21, Real e22)
    {
      m_rows[0].Set(e00, e01, e02);
      m_rows[1].Set(e10, e11, e12);
      m_rows[2].Set(e20, e21, e22);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetIdentity(void)
    {
      m_rows[0] = Vector3::c_xAxis;
      m_rows[1] = Vector3::c_yAxis;
      m_rows[2] = Vector3::c_zAxis;
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetRotationRad(Real x, Real y, Real z)
    {
      Real cx = MathUtils::Cos(x);
      Real cy = MathUtils::Cos(y);
      Real cz = MathUtils::Cos(z);
      Real sx = MathUtils::Sin(x);
      Real sy = MathUtils::Sin(y);
      Real sz = MathUtils::Sin(z);
      Real sxz = sx * sz;
      Real csxz = cz * sx;

      Set( (cz * cy) + (sxz * sy), (-sz * cy) + (csxz * sy), cx * sy,
                          sz * cx,                  cz * cx,     -sx,
          (cz * -sy) + (sxz * cy), (sz * sy) + (csxz * cy), cx * cy);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetRotationDeg(Real x, Real y, Real z)
    {
      SetRotationRad(x * SF_RADS_PER_DEG, y * SF_RADS_PER_DEG, z * SF_RADS_PER_DEG);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetRotation(const Quaternion &q)
    {
      Real s = q.LengthSq();
      s = Real(2.0) / s;

#ifdef SF_USE_SIMD
      __m128 row0, row1, row2;
      __m128 qData = q.Get128();
      // Y Y W
      row0 = _mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 0, 2, 2));
      // X X Z
      row1 = _mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 3, 1, 1));
      // W W Y
      row2 = _mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 2, 0, 0));

      // Y X Y Z
      __m128 temp = _mm_shuffle_ps(qData, qData, _MM_SHUFFLE(3, 2, 1, 2));
      row0 = _mm_mul_ps(row0, temp);
      row1 = _mm_mul_ps(row1, temp);
      row2 = _mm_mul_ps(row2, temp);

      row0 = _mm_xor_ps(row0, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));
      row1 = _mm_xor_ps(row1, _mm_set_ps(+0.0, -0.0, +0.0, +0.0));
      row2 = _mm_xor_ps(row2, _mm_set_ps(+0.0, +0.0, +0.0, -0.0));

      // Z Z X W
      temp = _mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 1, 3, 3));

      row0 = _mm_add_ps(row0,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 3, 0, 3)), temp));
      row1 = _mm_add_ps(row1,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, _MM_SHUFFLE(1, 0, 3, 0)), temp));
      row2 = _mm_add_ps(row2,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, _MM_SHUFFLE(0, 1, 2, 1)), temp));

      row0 = _mm_xor_ps(row0, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));
      row1 = _mm_xor_ps(row1, _mm_set_ps(+0.0, -0.0, +0.0, +0.0));

      __m128 scalar = _mm_set1_ps(s);
      row0 = _mm_mul_ps(scalar, row0);
      row1 = _mm_mul_ps(scalar, row1);
      row2 = _mm_mul_ps(scalar, row2);

      row0 = _mm_xor_ps(row0, _mm_set_ps(+0.0, +0.0, +0.0, -0.0));
      row1 = _mm_xor_ps(row1, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));
      row2 = _mm_xor_ps(row2, _mm_set_ps(+0.0, -0.0, +0.0, +0.0));

      row0 = _mm_add_ps(_mm_set_ps(0.0, 0.0, 0.0, 1.0), row0);
      row1 = _mm_add_ps(_mm_set_ps(0.0, 0.0, 1.0, 0.0), row1);
      row2 = _mm_add_ps(_mm_set_ps(0.0, 1.0, 0.0, 0.0), row2);

      m_rows[0].Set(row0);
      m_rows[1].Set(row1);
      m_rows[2].Set(row2);
#else
      Real xy = s * q[1] * q[2];
      Real wz = s * q[0] * q[3];
      Real xz = s * q[1] * q[3];
      Real wy = s * q[0] * q[2];
      Real yz = s * q[2] * q[3];
      Real wx = s * q[0] * q[1];
      Real yy = s * q[2] * q[2];
      Real xx = s * q[1] * q[1];
      Real zz = s * q[3] * q[3];

      Set(
        1 - yy - zz, xy - wz, xz + wy,
        xy + wz, 1 - xx - zz, yz - wx,
        xz - wy, yz + wx, 1 - xx - yy
      );
#endif
    }

    SF_FORCE_INLINE Quaternion GetQuaternion(void) const
    {
      Real trace = m_rows[0][0] + m_rows[1][1] + m_rows[2][2];
      Quaternion q;

      if (trace > 0)
      {
        Real s = MathUtils::Sqrt(Real(trace + 1.0)) * Real(2.0);
        return Quaternion(
          (Real)0.25 * s,
          (Real)(m_rows[2][1] - m_rows[1][2]) / s,
          (Real)(m_rows[0][2] - m_rows[2][0]) / s,
          (Real)(m_rows[1][0] - m_rows[0][1]) / s);
      }
      else if ((m_rows[0][0] > m_rows[1][1]) && (m_rows[0][0] > m_rows[2][2]))
      {
        Real s = MathUtils::Sqrt(Real(1.0 + m_rows[0][0] - m_rows[1][1] - m_rows[2][2])) * 2;
        return Quaternion(
          (Real)(m_rows[2][1] - m_rows[1][2]) / s,
          (Real)0.25 * s,
          (Real)(m_rows[0][1] + m_rows[1][0]) / s,
          (Real)(m_rows[0][2] + m_rows[2][0]) / s
        );
      }
      else if (m_rows[1][1] > m_rows[2][2])
      {
        Real s = MathUtils::Sqrt(Real(1.0 + m_rows[1][1] - m_rows[0][0] - m_rows[2][2])) * 2;
        return Quaternion(
          (Real)(m_rows[0][2] - m_rows[2][0]) / s,
          (Real)(m_rows[0][1] + m_rows[1][0]) / s,
          (Real)0.25 * s,
          (Real)(m_rows[1][2] + m_rows[2][1]) / s
        );
      }
      else
      {
        Real s = MathUtils::Sqrt(Real(1.0 + m_rows[2][2] - m_rows[0][0] - m_rows[1][1])) * 2;
        return Quaternion(
          (Real)(m_rows[1][0] - m_rows[0][1]) / s,
          (Real)(m_rows[0][2] + m_rows[2][0]) / s,
          (Real)(m_rows[1][2] + m_rows[2][1]) / s,
          (Real)0.25 * s
        );
      }
    }

    SF_FORCE_INLINE Real SF_VEC_CALL Determinant(void) const
    {
#ifdef SF_USE_SIMD
      __m128 c1 = _mm_shuffle_ps(m_rows[1].Get128(), m_rows[1].Get128(), 0xC9);
      __m128 c2 = _mm_shuffle_ps(m_rows[2].Get128(), m_rows[2].Get128(), 0xC9); //_MM_SHUFFLE(3, 0, 2, 1)

      c1 = _mm_mul_ps(c1, m_rows[2].Get128());
      c2 = _mm_mul_ps(c2, m_rows[1].Get128());
      
      c1 = _mm_sub_ps(c2, c1);

      c1 = _mm_shuffle_ps(c1, c1, 0xC9);
      c1 = _mm_mul_ps(m_rows[0].Get128(), c1);

      __m128 z = _mm_movehl_ps(c1, c1);
      __m128 y = _mm_shuffle_ps(c1, c1, 0xFD);  //_MM_SHUFFLE(3, 3, 3, 1)
      c1 = _mm_add_ps(z, _mm_add_ps(y, c1));

      return _mm_cvtss_f32(c1);
#else
      return m_rows[0].GetX() * (m_rows[1].GetY() * m_rows[2].GetZ()
                                 - m_rows[2].GetY() * m_rows[1].GetZ())
             - m_rows[1].GetX() * (m_rows[0].GetY() * m_rows[2].GetZ()
                                   - m_rows[2].GetY() * m_rows[0].GetZ())
             + m_rows[2].GetX() * (m_rows[0].GetY() * m_rows[1].GetZ()
                                   - m_rows[1].GetY() * m_rows[0].GetZ());
#endif
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL Transposed(void) const
    {
#ifdef SF_USE_SIMD
      __m128 temp = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());


      __m128 v1 = _mm_shuffle_ps(v0, m_rows[2].Get128(), 0xDE); //_MM_SHUFFLE(3, 1, 3, 2)
      __m128 v2 = _mm_shuffle_ps(temp, m_rows[2].Get128(), 0xE4); //_MM_SHUFFLE(3, 2, 1, 0)
      v0 = _mm_shuffle_ps(v0, m_rows[2].Get128(), 0xC4); //_MM_SHUFFLE(3, 0, 1, 0)

      return Matrix3(v0, v1, v2);
#else
      return Matrix3(m_rows[0].GetX(), m_rows[1].GetX(), m_rows[2].GetX(),
        m_rows[0].GetY(), m_rows[1].GetY(), m_rows[2].GetY(),
        m_rows[0].GetZ(), m_rows[1].GetZ(), m_rows[2].GetZ());
#endif
    }

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL Transpose(void)
    {
      *this = Transposed();

      return *this;
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL GetCofactorMatrix(void) const
    {
      return Matrix3(_Cofactor(1, 1, 2, 2), -_Cofactor(1, 0, 2, 2), _Cofactor(1, 0, 2, 1),
        -_Cofactor(0, 1, 2, 2), _Cofactor(0, 0, 2, 2), -_Cofactor(0, 0, 2, 1),
        _Cofactor(0, 1, 1, 2), -_Cofactor(0, 0, 1, 2), _Cofactor(0, 0, 1, 1));
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL GetAdjointMatrix(void) const
    {
      return Matrix3(_Cofactor(1, 1, 2, 2), -_Cofactor(0, 1, 2, 2), _Cofactor(0, 1, 1, 2),
        -_Cofactor(1, 0, 2, 2), _Cofactor(0, 0, 2, 2), -_Cofactor(0, 0, 1, 2),
        _Cofactor(1, 0, 2, 1), -_Cofactor(0, 0, 2, 1), _Cofactor(0, 0, 1, 1));
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL Inverted(void) const
    {
      Real det = Determinant();
      assert(det != 0.0);
      Real s = Real(1.0) / det;

      return Matrix3(_Cofactor(1, 1, 2, 2) * s, -_Cofactor(0, 1, 2, 2) * s, _Cofactor(0, 1, 1, 2) * s,
        -_Cofactor(1, 0, 2, 2) * s, _Cofactor(0, 0, 2, 2) * s, -_Cofactor(0, 0, 1, 2) * s,
        _Cofactor(1, 0, 2, 1) * s, -_Cofactor(0, 0, 2, 1) * s, _Cofactor(0, 0, 1, 1) * s);
    }

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL Inverse(void)
    {
      *this = Inverted();

      return *this;
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL GetColumn(int i) const
    {
      assert(0 <= i && i < 3);
      return Vector3(m_rows[0][i], m_rows[1][i], m_rows[2][i]);
    }

    SF_FORCE_INLINE Vector3& SF_VEC_CALL operator[](int i)
    {
      assert(0 <= i && i < 3);
      return m_rows[i];
    }

    SF_FORCE_INLINE const Vector3& SF_VEC_CALL operator[](int i) const
    {
      assert(0 <= i && i < 3);
      return m_rows[i];
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL operator+(const Matrix3 &other) const
    {
      return Matrix3(m_rows[0] + other.m_rows[0], m_rows[1] + other.m_rows[1],
        m_rows[2] + other.m_rows[2]);
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL operator-(const Matrix3 &other) const
    {
      return Matrix3(m_rows[0] - other.m_rows[0], m_rows[1] - other.m_rows[1],
        m_rows[2] - other.m_rows[2]);
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL operator*(const Matrix3 &other) const
    {
#ifdef SF_USE_SIMD
      __m128 tv0 = m_rows[0].Get128();
      __m128 tv1 = m_rows[1].Get128();
      __m128 tv2 = m_rows[2].Get128();
      __m128 mv0 = other.m_rows[0].Get128();
      __m128 mv1 = other.m_rows[1].Get128();
      __m128 mv2 = other.m_rows[2].Get128();

      __m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
      __m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
      __m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);

      v00 = _mm_mul_ps(v00, mv0);
      v01 = _mm_mul_ps(v01, mv1);
      v02 = _mm_mul_ps(v02, mv2);

      __m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
      __m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
      __m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);

      v10 = _mm_mul_ps(v10, mv0);
      v11 = _mm_mul_ps(v11, mv1);
      v12 = _mm_mul_ps(v12, mv2);

      __m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
      __m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
      __m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);

      v20 = _mm_mul_ps(v20, mv0);
      v21 = _mm_mul_ps(v21, mv1);
      v22 = _mm_mul_ps(v22, mv2);

      return Matrix3(_mm_add_ps(v00, _mm_add_ps(v01, v02)),
        _mm_add_ps(v10, _mm_add_ps(v11, v12)),
        _mm_add_ps(v20, _mm_add_ps(v21, v22)));
#else
      return Matrix3(m_rows[0].Dot(other.GetColumn(0)), m_rows[0].Dot(other.GetColumn(1)), m_rows[0].Dot(other.GetColumn(2)),
        m_rows[1].Dot(other.GetColumn(0)), m_rows[1].Dot(other.GetColumn(1)), m_rows[1].Dot(other.GetColumn(2)),
        m_rows[2].Dot(other.GetColumn(0)), m_rows[2].Dot(other.GetColumn(1)), m_rows[2].Dot(other.GetColumn(2)));
#endif
    }

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL operator*=(const Matrix3 &other)
    {
#ifdef SF_USE_SIMD
      __m128 tv0 = m_rows[0].Get128();
      __m128 tv1 = m_rows[1].Get128();
      __m128 tv2 = m_rows[2].Get128();
      __m128 mv0 = other.m_rows[0].Get128();
      __m128 mv1 = other.m_rows[1].Get128();
      __m128 mv2 = other.m_rows[2].Get128();

      __m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
      __m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
      __m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);

      v00 = _mm_mul_ps(v00, mv0);
      v01 = _mm_mul_ps(v01, mv1);
      v02 = _mm_mul_ps(v02, mv2);

      __m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
      __m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
      __m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);

      v10 = _mm_mul_ps(v10, mv0);
      v11 = _mm_mul_ps(v11, mv1);
      v12 = _mm_mul_ps(v12, mv2);

      __m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
      __m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
      __m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);

      v20 = _mm_mul_ps(v20, mv0);
      v21 = _mm_mul_ps(v21, mv1);
      v22 = _mm_mul_ps(v22, mv2);

      m_rows[0] = Vector3(_mm_add_ps(v00, _mm_add_ps(v01, v02)));
      m_rows[1] = Vector3(_mm_add_ps(v10, _mm_add_ps(v11, v12)));
      m_rows[2] = Vector3(_mm_add_ps(v20, _mm_add_ps(v21, v22)));
#else
      Set(m_rows[0].Dot(other.GetColumn(0)), m_rows[0].Dot(other.GetColumn(1)), m_rows[0].Dot(other.GetColumn(2)),
        m_rows[1].Dot(other.GetColumn(0)), m_rows[1].Dot(other.GetColumn(1)), m_rows[1].Dot(other.GetColumn(2)),
        m_rows[2].Dot(other.GetColumn(0)), m_rows[2].Dot(other.GetColumn(1)), m_rows[2].Dot(other.GetColumn(2)));
#endif
      return *this;
    }

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL operator+=(const Matrix3 &other)
    {
      m_rows[0] += other.m_rows[0];
      m_rows[1] += other.m_rows[1];
      m_rows[2] += other.m_rows[2];

      return *this;
    }

    SF_FORCE_INLINE Matrix3& SF_VEC_CALL operator-=(const Matrix3 &other)
    {
      m_rows[0] -= other.m_rows[0];
      m_rows[1] -= other.m_rows[1];
      m_rows[2] -= other.m_rows[2];

      return *this;
    }

    SF_FORCE_INLINE Matrix3 SF_VEC_CALL operator*(Real s) const
    {
      return Matrix3(m_rows[0] * s, m_rows[1] * s, m_rows[2] * s);
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator*(const Vector3 &v) const
    {
      return Vector3(m_rows[0].Dot(v), m_rows[1].Dot(v), m_rows[2].Dot(v));
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator==(const Matrix3 &other) const
    {
      return (m_rows[0] == other.m_rows[0]) && (m_rows[1] == other.m_rows[1])
        && (m_rows[2] == other.m_rows[2]);
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator!=(const Matrix3 &other) const
    {
      return !(*this == other);
    }

  private:
    SF_FORCE_INLINE Real _Cofactor(int r1, int c1, int r2, int c2) const
    {
      return m_rows[r1][c1] * m_rows[r2][c2] - m_rows[r2][c1] * m_rows[r1][c2];
    }

    Vector3 m_rows[3];
  };

  SF_FORCE_INLINE void SF_VEC_CALL SetIdentity(Matrix3 &m)
  {
    m.SetIdentity();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL GetDeterminant(const Matrix3 &m)
  {
    return m.Determinant();
  }

  SF_FORCE_INLINE Matrix3 SF_VEC_CALL GetTranspose(const Matrix3 &m)
  {
    return m.Transposed();
  }

  SF_FORCE_INLINE Matrix3& SF_VEC_CALL Transpose(Matrix3 &m)
  {
    return m.Transpose();
  }

  SF_FORCE_INLINE Matrix3 SF_VEC_CALL GetInverse(const Matrix3 &m)
  {
    return m.Inverted();
  }

  SF_FORCE_INLINE Matrix3& SF_VEC_CALL Inverse(Matrix3 &m)
  {
    return m.Inverse();
  }

} //Sulfur
