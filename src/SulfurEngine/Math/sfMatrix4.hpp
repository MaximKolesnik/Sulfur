/******************************************************************************/
/*!
\par     Sulfur
\file    sfMatrix4.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/22/2016

\brief   4x4 Matrix

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../sfProjectDefs.hpp"
#include "sfMathDefs.hpp"
#include "sfVector4.hpp"
#include "sfVector3.hpp"
#include "sfQuaternion.hpp"

namespace Sulfur
{
  SF_ATTR_ALIGN_16 class Matrix4
  {
  public:
    static const Matrix4 SF_ATTR_ALIGN_16 c_identity;

    SF_FORCE_INLINE SF_VEC_CALL Matrix4(void)
    {
    }

    SF_FORCE_INLINE SF_VEC_CALL Matrix4(const Matrix4 &other)
    {
      m_rows[0] = other.m_rows[0];
      m_rows[1] = other.m_rows[1];
      m_rows[2] = other.m_rows[2];
      m_rows[3] = other.m_rows[3];
    }

    SF_FORCE_INLINE SF_VEC_CALL Matrix4(const Quaternion &q)
    {
      Real s = q.Length();
      SF_ASSERT(s != 0, "Quaternion of length 0");
      s = Real(2.0) / s;

#ifdef SF_USE_SIMD
      __m128 row0, row1, row2;
      __m128 qData = q.Get128();
      // Y Y W
      row0 = _mm_shuffle_ps(qData, qData, 0xA); //_MM_SHUFFLE(0, 0, 2, 2)
      // X X Z
      row1 = _mm_shuffle_ps(qData, qData, 0x35); //_MM_SHUFFLE(0, 3, 1, 1)
      // W W Y
      row2 = _mm_shuffle_ps(qData, qData, 0x20); //_MM_SHUFFLE(0, 2, 0, 0)

      // Y X Y Z
      __m128 temp = _mm_shuffle_ps(qData, qData, 0xE6); //_MM_SHUFFLE(3, 2, 1, 2)
      row0 = _mm_mul_ps(row0, temp);
      row1 = _mm_mul_ps(row1, temp);
      row2 = _mm_mul_ps(row2, temp);

      row0 = _mm_xor_ps(row0, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));
      row1 = _mm_xor_ps(row1, _mm_set_ps(+0.0, -0.0, +0.0, +0.0));
      row2 = _mm_xor_ps(row2, _mm_set_ps(+0.0, +0.0, +0.0, -0.0));

      // Z Z X W
      temp = _mm_shuffle_ps(qData, qData, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      row0 = _mm_add_ps(row0,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x33), temp)); //_MM_SHUFFLE(0, 3, 0, 3)
      row1 = _mm_add_ps(row1,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x4C), temp)); //_MM_SHUFFLE(1, 0, 3, 0)
      row2 = _mm_add_ps(row2,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x19), temp)); //_MM_SHUFFLE(0, 1, 2, 1)

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
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
      m_rows[0][3] = 0.0;
      m_rows[1][3] = 0.0;
      m_rows[2][3] = 0.0;
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
        1 - yy - zz, xy - wz, xz + wy, 0.0,
        xy + wz, 1 - xx - zz, yz - wx, 0.0,
        xz - wy, yz + wx, 1 - xx - yy, 0.0,
        0.0, 0.0, 0.0, 1.0
        );
#endif
    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Matrix4(Real e00, Real e01, Real e02, Real e03,
                                        Real e10, Real e11, Real e12, Real e13,
                                        Real e20, Real e21, Real e22, Real e23,
                                        Real e30, Real e31, Real e32, Real e33)
    {
      m_rows[0].Set(e00, e01, e02, e03);
      m_rows[1].Set(e10, e11, e12, e13);
      m_rows[2].Set(e20, e21, e22, e23);
      m_rows[3].Set(e30, e31, e32, e33);
    }

    SF_FORCE_INLINE explicit SF_VEC_CALL Matrix4(const Vector4 &v0, const Vector4 &v1,
      const Vector4 &v2, const Vector4 &v3)
    {
      m_rows[0] = v0;
      m_rows[1] = v1;
      m_rows[2] = v2;
      m_rows[3] = v3;
    }

    // Will set e33 to 1.0
    SF_FORCE_INLINE explicit SF_VEC_CALL Matrix4(const Vector3 &v0, const Vector3 &v1,
      const Vector3 &v2, const Vector3 &v3)
    {
      m_rows[0].Set(v0.GetX(), v1.GetX(), v2.GetX(), v3.GetX());
      m_rows[1].Set(v0.GetY(), v1.GetY(), v2.GetY(), v3.GetY());
      m_rows[2].Set(v0.GetZ(), v1.GetZ(), v2.GetZ(), v3.GetZ());
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
    }

#ifdef SF_USE_SIMD
    SF_FORCE_INLINE explicit SF_VEC_CALL Matrix4(__m128 v0, __m128 v1,
      __m128 v2, __m128 v3)
    {
      m_rows[0] = Vector4(v0);
      m_rows[1] = Vector4(v1);
      m_rows[2] = Vector4(v2);
      m_rows[3] = Vector4(v3);
    }
#endif

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL operator=(const Matrix4 &other)
    {
      m_rows[0] = other.m_rows[0];
      m_rows[1] = other.m_rows[1];
      m_rows[2] = other.m_rows[2];
      m_rows[3] = other.m_rows[3];

      return *this;
    }

    SF_FORCE_INLINE void SF_VEC_CALL Set(Real e00, Real e01, Real e02, Real e03,
      Real e10, Real e11, Real e12, Real e13,
      Real e20, Real e21, Real e22, Real e23,
      Real e30, Real e31, Real e32, Real e33)
    {
      m_rows[0].Set(e00, e01, e02, e03);
      m_rows[1].Set(e10, e11, e12, e13);
      m_rows[2].Set(e20, e21, e22, e23);
      m_rows[3].Set(e30, e31, e32, e33);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetIdentity(void)
    {
      m_rows[0].Set(1.0, 0.0, 0.0, 0.0);
      m_rows[1].Set(0.0, 1.0, 0.0, 0.0);
      m_rows[2].Set(0.0, 0.0, 1.0, 0.0);
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
    }

    //XZY
    SF_FORCE_INLINE void SF_VEC_CALL SetRotationRad(Real roll, Real pitch, Real yaw)
    {
      Real cx = MathUtils::Cos(yaw);  //heading
      Real cy = MathUtils::Cos(pitch);//attitude
      Real cz = MathUtils::Cos(roll); //bank
      Real sx = MathUtils::Sin(yaw);
      Real sy = MathUtils::Sin(pitch);
      Real sz = MathUtils::Sin(roll);

      Set(cx * cy, (-cx * sy * cz) + (sx * sz), (cx * sy * sz) + (sx * cz), 0.0,
          sy,  cy * cz, -cy * sz, 0.0,
         -sx * cy, (sx * sy * cz) + (cx * sz), (-sx * sy * sz) + (cx * cz), 0.0,
                             0.0,                      0.0,     0.0, 1.0);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetScaling(Real x, Real y, Real z)
    {
      m_rows[0].Set(x, 0.0, 0.0, 0.0);
      m_rows[1].Set(0.0, y, 0.0, 0.0);
      m_rows[2].Set(0.0, 0.0, z, 0.0);
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetTranslation(Real x, Real y, Real z)
    {
      m_rows[0].Set(1.0, 0.0, 0.0, x);
      m_rows[1].Set(0.0, 1.0, 0.0, y);
      m_rows[2].Set(0.0, 0.0, 1.0, z);
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
    }

    //XZY
    SF_FORCE_INLINE void SF_VEC_CALL SetTransformation(const Vector3 &euler,
      const Vector3 &scaling, const Vector3 &trans)
    {
      Matrix4 m1, m2;
      Real cx = MathUtils::Cos(euler[0]);
      Real cy = MathUtils::Cos(euler[1]);
      Real cz = MathUtils::Cos(euler[2]);
      Real sx = MathUtils::Sin(euler[0]);
      Real sy = MathUtils::Sin(euler[1]);
      Real sz = MathUtils::Sin(euler[2]);
      Real sxz = sx * sz;
      Real csxz = cz * sx;

      m1.Set(cx * cy, (-cx * sy * cz) + (sx * sz), (cx * sy * sz) + (sx * cz), trans[0],
        sy, cy * cz, -cy * sz, trans[1],
        -sx * cy, (sx * sy * cz) + (cx * sz), (-sx * sy * sz) + (cx * cz), trans[2],
        0.0, 0.0, 0.0, 1.0);
      m2.SetScaling(scaling[0], scaling[1], scaling[2]);

      *this = (m1 * m2);
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetTransformation(const Quaternion &rotation,
      const Vector3 &scaling, const Vector3 &trans)
    {
      Matrix4 r(rotation), s, t;
      s.SetScaling(scaling[0], scaling[1], scaling[2]);
      t.SetTranslation(trans[0], trans[1], trans[2]);

      *this = t * r * s;
    }

    SF_FORCE_INLINE void SF_VEC_CALL SetRotationDeg(Real roll, Real pitch, Real yaw)
    {
      SetRotationRad(roll * SF_RADS_PER_DEG, pitch * SF_RADS_PER_DEG,
        yaw * SF_RADS_PER_DEG);
    }

    SF_FORCE_INLINE void SetRotation(const Quaternion &q)
    {
      Real s = q.Length();
      SF_ASSERT(s != 0, "Quaternion of length 0");
      s = Real(2.0) / s;

#ifdef SF_USE_SIMD
      __m128 row0, row1, row2;
      __m128 qData = q.Get128();
      // Y Y W
      row0 = _mm_shuffle_ps(qData, qData, 0xA); //_MM_SHUFFLE(0, 0, 2, 2)
      // X X Z
      row1 = _mm_shuffle_ps(qData, qData, 0x35); //_MM_SHUFFLE(0, 3, 1, 1)
      // W W Y
      row2 = _mm_shuffle_ps(qData, qData, 0x20); //_MM_SHUFFLE(0, 2, 0, 0)

      // Y X Y Z
      __m128 temp = _mm_shuffle_ps(qData, qData, 0xE6); //_MM_SHUFFLE(3, 2, 1, 2)
      row0 = _mm_mul_ps(row0, temp);
      row1 = _mm_mul_ps(row1, temp);
      row2 = _mm_mul_ps(row2, temp);

      row0 = _mm_xor_ps(row0, _mm_set_ps(+0.0, +0.0, -0.0, +0.0));
      row1 = _mm_xor_ps(row1, _mm_set_ps(+0.0, -0.0, +0.0, +0.0));
      row2 = _mm_xor_ps(row2, _mm_set_ps(+0.0, +0.0, +0.0, -0.0));

      // Z Z X W
      temp = _mm_shuffle_ps(qData, qData, 0x1F); //_MM_SHUFFLE(0, 1, 3, 3)

      row0 = _mm_add_ps(row0,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x33), temp)); //_MM_SHUFFLE(0, 3, 0, 3)
      row1 = _mm_add_ps(row1,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x4C), temp)); //_MM_SHUFFLE(1, 0, 3, 0)
      row2 = _mm_add_ps(row2,
        _mm_mul_ps(_mm_shuffle_ps(qData, qData, 0x19), temp)); //_MM_SHUFFLE(0, 1, 2, 1)

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
      m_rows[3].Set(0.0, 0.0, 0.0, 1.0);
      m_rows[0][3] = 0.0;
      m_rows[1][3] = 0.0;
      m_rows[2][3] = 0.0;
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
        1 - yy - zz, xy - wz, xz + wy, 0.0,
        xy + wz, 1 - xx - zz, yz - wx, 0.0,
        xz - wy, yz + wx, 1 - xx - yy, 0.0,
        0.0, 0.0, 0.0, 1.0
      );
#endif
    }

    SF_FORCE_INLINE Quaternion SF_VEC_CALL GetQuaternion(void) const
    {
      Real trace = m_rows[0][0] + m_rows[1][1] + m_rows[2][2];
      Quaternion q;

      if (trace > 0)
      {
        Real s = Real(0.5) / MathUtils::Sqrt(Real(trace + 1.0));
        return Quaternion(
          (Real)0.25 / s,
          (Real)(m_rows[2][1] - m_rows[1][2]) * s,
          (Real)(m_rows[0][2] - m_rows[2][0]) * s,
          (Real)(m_rows[1][0] - m_rows[0][1]) * s);
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

    //No unit test
    SF_FORCE_INLINE void SF_VEC_CALL SetLookAtLH(const Vector3& eye, 
      const Vector3& lookAt, const Vector3& up)
    {
      Vector3 zaxis = lookAt - eye;
      zaxis.Normalize();

      Vector3 xaxis = up.Cross(zaxis);
      xaxis.Normalize();

      Vector3 yaxis = zaxis.Cross(xaxis);

      Set(
        xaxis[0], xaxis[1], xaxis[2], -xaxis.Dot(eye),
        yaxis[0], yaxis[1], yaxis[2], -yaxis.Dot(eye),
        zaxis[0], zaxis[1], zaxis[2], -zaxis.Dot(eye),
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    //No unit test
    SF_FORCE_INLINE void SF_VEC_CALL SetViewMatrix(const Vector3& right, 
      const Vector3& up, const Vector3& forward, const Vector3& eye)
    {
      Set(
        right[0], right[1], right[2], right.Dot(-eye),
        up[0], up[1], up[2], up.Dot(-eye),
        forward[0], forward[1], forward[2], forward.Dot(-eye),
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    //No unit test
    SF_FORCE_INLINE void SF_VEC_CALL SetOrthographicLH(float width, 
      float height, float nearPlane, float farPlane)
    {
      Set(
        2.0f / width, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f / (farPlane - nearPlane), nearPlane / (nearPlane - farPlane),
        0.0f, 0.0f, 0.0f, 1.0f
      );
    }

    //No unit test
    SF_FORCE_INLINE void SF_VEC_CALL SetPerspectiveFovLH(float width, 
      float height, float fov, float nearPlane, float farPlane)
    {
      float aspect = width / height;
      float yScale = 1.0f / tanf(fov / 2.0f);
      float xScale = yScale / aspect;
      float range = farPlane - nearPlane;

      Set(
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, farPlane / range, -farPlane * nearPlane / range,
        0.0f, 0.0f, 1.0f, 0.0f
      );
    }

    // Calculate determinant, expanding the first column
    // Use short determinant, if the matrix is a transformation matrix
    SF_FORCE_INLINE Real SF_VEC_CALL DeterminantFull(void) const
    {
#ifdef SF_USE_SIMD
      __m128 row1 = _mm_shuffle_ps(m_rows[1].Get128(), m_rows[1].Get128(), 0x4E);
      __m128 row3 = _mm_shuffle_ps(m_rows[3].Get128(), m_rows[3].Get128(), 0x4E);

      __m128 tmp = _mm_mul_ps(m_rows[2].Get128(), row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1); // _MM_SHUFFLE(2, 3, 0, 1)

      __m128 minor = _mm_mul_ps(tmp, row1);
      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E); //_MM_SHUFFLE(1, 0, 3, 2)
      minor = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor);

      tmp = _mm_mul_ps(row1, m_rows[2].Get128());
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor = _mm_add_ps(_mm_mul_ps(row3, tmp), minor);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
      minor = _mm_sub_ps(minor, _mm_mul_ps(row3, tmp));

      tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
      __m128 row2 = _mm_shuffle_ps(m_rows[2].Get128(), m_rows[2].Get128(), 0x4E);
      minor = _mm_add_ps(_mm_mul_ps(row2, tmp), minor);
      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor = _mm_sub_ps(minor, _mm_mul_ps(row2, tmp));

      __m128 det = _mm_mul_ps(m_rows[0].Get128(), minor);
      det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
      det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);

      return _mm_cvtss_f32(det);
#else
      Real c0 = m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c1 = m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c2 = m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c3 = m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);

      return m_rows[0][0] * c0 - m_rows[1][0] * c1 + m_rows[2][0] * c2 
        - m_rows[3][0] * c3;
#endif
    }

    //Assumes matrix is affine transformation matrix
    SF_FORCE_INLINE Real SF_VEC_CALL DeterminantShort(void) const
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
      return m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

#endif
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL Transposed(void) const
    {
#ifdef SF_USE_SIMD
      __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 v0    = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
      __m128 v2    = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

      __m128 v1    = _mm_shuffle_ps(v0, v2, 0xEE);
      __m128 v3    = _mm_shuffle_ps(temp1, temp2, 0xEE); // _MM_SHUFFLE(3, 2, 3, 2)
             v0    = _mm_shuffle_ps(v0, v2, 0x44);
             v2    = _mm_shuffle_ps(temp1, temp2, 0x44); //_MM_SHUFFLE(1, 0, 1, 0)

      return Matrix4(v0, v1, v2, v3);
#else
      return Matrix4(m_rows[0][0], m_rows[1][0], m_rows[2][0], m_rows[3][0],
        m_rows[0][1], m_rows[1][1], m_rows[2][1], m_rows[3][1],
        m_rows[0][2], m_rows[1][2], m_rows[2][2], m_rows[3][2],
        m_rows[0][3], m_rows[1][3], m_rows[2][3], m_rows[3][3]);
#endif
    }

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL Transpose(void)
    {
#ifdef SF_USE_SIMD
      __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
      __m128 v2 = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

      m_rows[1].Set(_mm_shuffle_ps(v0, v2, 0xEE));
      m_rows[3].Set(_mm_shuffle_ps(temp1, temp2, 0xEE));
      m_rows[0].Set(_mm_shuffle_ps(v0, v2, 0x44));
      m_rows[2].Set(_mm_shuffle_ps(temp1, temp2, 0x44));
#else
      *this = Transposed();
#endif

      return *this;
    }
    
    //Provided by intel
    //download.intel.com/design/pentiumiii/sml/24504301.pdf
    SF_FORCE_INLINE Matrix4 SF_VEC_CALL GetCofactorMatrix(void) const
    {
#ifdef SF_USE_SIMD
      __m128 row0 = m_rows[0].Get128();
      __m128 row1 = m_rows[1].Get128();
      __m128 row2 = m_rows[2].Get128();
      __m128 row3 = m_rows[3].Get128();

      row1 = _mm_shuffle_ps(row1, row1, 0x4E);
      row3 = _mm_shuffle_ps(row3, row3, 0x4E);

      __m128 tmp = _mm_mul_ps(row2, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      __m128 minor0 = _mm_mul_ps(row1, tmp);
      __m128 minor1 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
      minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
      minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

      tmp = _mm_mul_ps(row1, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
      __m128 minor3 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
      minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
      minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

      tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
      row2 = _mm_shuffle_ps(row2, row2, 0x4E);

      minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
      __m128 minor2 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
      minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
      minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

      tmp = _mm_mul_ps(row0, row1);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

      tmp = _mm_mul_ps(row0, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
      minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
      minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

      tmp = _mm_mul_ps(row0, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
      minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

      /*minor0 = _mm_xor_ps(minor0, _mm_set_ps(-0.0, +0.0, -0.0, +0.0));
      minor1 = _mm_xor_ps(minor1, _mm_set_ps(+0.0, -0.0, +0.0, -0.0));
      minor2 = _mm_xor_ps(minor2, _mm_set_ps(-0.0, +0.0, -0.0, +0.0));
      minor3 = _mm_xor_ps(minor3, _mm_set_ps(+0.0, -0.0, +0.0, -0.0));*/

      return Matrix4(minor0, minor1, minor2, minor3);
#else
      Real c00 = m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c01 = m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c02 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
      Real c03 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
      Real c10 = m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c11 = m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c12 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
      Real c13 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
      Real c20 = m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c21 = m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c22 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
      Real c23 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
      Real c30 = m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c31 = m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c32 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
      Real c33 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

      return Matrix4(c00, -c01, c02, -c03,
        -c10, c11, -c12, c13,
        c20, -c21, c22, -c23,
        -c30, c31, -c32, c33);
#endif
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL GetAdjointMatrix(void) const
    {
#ifdef SF_USE_SIMD
      __m128 row0 = m_rows[0].Get128();
      __m128 row1 = m_rows[1].Get128();
      __m128 row2 = m_rows[2].Get128();
      __m128 row3 = m_rows[3].Get128();

      row1 = _mm_shuffle_ps(row1, row1, 0x4E);
      row3 = _mm_shuffle_ps(row3, row3, 0x4E);

      __m128 tmp = _mm_mul_ps(row2, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      __m128 minor0 = _mm_mul_ps(row1, tmp);
      __m128 minor1 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
      minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
      minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

      tmp = _mm_mul_ps(row1, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
      __m128 minor3 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
      minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
      minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

      tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
      row2 = _mm_shuffle_ps(row2, row2, 0x4E);

      minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
      __m128 minor2 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
      minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
      minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

      tmp = _mm_mul_ps(row0, row1);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

      tmp = _mm_mul_ps(row0, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
      minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
      minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

      tmp = _mm_mul_ps(row0, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
      minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

      __m128 temp1 = _mm_unpackhi_ps(minor0, minor1);
      __m128 v0 = _mm_unpacklo_ps(minor0, minor1);
      __m128 temp2 = _mm_unpackhi_ps(minor2, minor3);
      __m128 v2 = _mm_unpacklo_ps(minor2, minor3);

      return Matrix4(_mm_shuffle_ps(v0, v2, 0x44), _mm_shuffle_ps(v0, v2, 0xEE),
        _mm_shuffle_ps(temp1, temp2, 0x44), _mm_shuffle_ps(temp1, temp2, 0xEE));
#else
      Real c00 = m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c01 = m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c02 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
      Real c03 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
      Real c10 = m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c11 = m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c12 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
      Real c13 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
      Real c20 = m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c21 = m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c22 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
      Real c23 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
      Real c30 = m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c31 = m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c32 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
      Real c33 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

      return Matrix4(c00, -c10, c20, -c30,
        -c01, c11, -c21, c31,
        c02, -c12, c22, -c32,
        -c03, c13, -c23, c33);
#endif
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL Inverted(void) const
    {

      SF_ASSERT(DeterminantFull() != 0.0, "Determinant is zero");


#ifdef SF_USE_SIMD
      __m128 temp1 = _mm_unpackhi_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 v0 = _mm_unpacklo_ps(m_rows[0].Get128(), m_rows[1].Get128());
      __m128 temp2 = _mm_unpackhi_ps(m_rows[2].Get128(), m_rows[3].Get128());
      __m128 v2 = _mm_unpacklo_ps(m_rows[2].Get128(), m_rows[3].Get128());

      __m128 row1 = _mm_shuffle_ps(v0, v2, 0xEE);
      __m128 row3 = _mm_shuffle_ps(temp1, temp2, 0xEE);
      __m128 row0 = _mm_shuffle_ps(v0, v2, 0x44);
      __m128 row2 = _mm_shuffle_ps(temp1, temp2, 0x44);

      row1 = _mm_shuffle_ps(row1, row1, 0x4E);
      row3 = _mm_shuffle_ps(row3, row3, 0x4E);

      __m128 tmp = _mm_mul_ps(row2, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      __m128 minor0 = _mm_mul_ps(row1, tmp);
      __m128 minor1 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
      minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
      minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

      tmp = _mm_mul_ps(row1, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
      __m128 minor3 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
      minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
      minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

      tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
      row2 = _mm_shuffle_ps(row2, row2, 0x4E);

      minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
      __m128 minor2 = _mm_mul_ps(row0, tmp);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
      minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
      minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

      tmp = _mm_mul_ps(row0, row1);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));

      tmp = _mm_mul_ps(row0, row3);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
      minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
      minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));

      tmp = _mm_mul_ps(row0, row2);
      tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);

      minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
      minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));

      tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);

      minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
      minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);

      __m128 det = _mm_mul_ps(row0, minor0);
      det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
      det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
      tmp = _mm_rcp_ss(det);
      det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
      det = _mm_shuffle_ps(det, det, 0x00);

      minor0 = _mm_mul_ps(det, minor0);
      minor1 = _mm_mul_ps(det, minor1);
      minor2 = _mm_mul_ps(det, minor2);
      minor3 = _mm_mul_ps(det, minor3);

      return Matrix4(minor0, minor1, minor2, minor3);
#else
      Real c00 = m_rows[1][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c01 = m_rows[1][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3]);
      Real c02 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][1] * m_rows[1][3])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3]);
      Real c03 = m_rows[1][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][1] * m_rows[1][2])
        + m_rows[3][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2]);
      Real c10 = m_rows[0][1] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c11 = m_rows[0][0] * (m_rows[2][2] * m_rows[3][3] - m_rows[3][2] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3]);
      Real c12 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][3] - m_rows[3][1] * m_rows[2][3])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][1] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3]);
      Real c13 = m_rows[0][0] * (m_rows[2][1] * m_rows[3][2] - m_rows[3][1] * m_rows[2][2])
        - m_rows[2][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][1] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2]);
      Real c20 = m_rows[0][1] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c21 = m_rows[0][0] * (m_rows[1][2] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[1][3]);
      Real c22 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][3] - m_rows[3][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][3] - m_rows[3][2] * m_rows[0][3])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[0][3]);
      Real c23 = m_rows[0][0] * (m_rows[1][1] * m_rows[3][2] - m_rows[3][2] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[3][2] - m_rows[3][2] * m_rows[0][2])
        + m_rows[3][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);
      Real c30 = m_rows[0][1] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][1] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][1] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c31 = m_rows[0][0] * (m_rows[1][2] * m_rows[2][3] - m_rows[2][2] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][2] * m_rows[2][3] - m_rows[2][2] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][2] * m_rows[1][3] - m_rows[1][2] * m_rows[0][3]);
      Real c32 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][3] - m_rows[2][1] * m_rows[1][3])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][3] - m_rows[2][1] * m_rows[0][3])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][3] - m_rows[1][1] * m_rows[1][3]);
      Real c33 = m_rows[0][0] * (m_rows[1][1] * m_rows[2][2] - m_rows[2][1] * m_rows[1][2])
        - m_rows[1][0] * (m_rows[0][1] * m_rows[2][2] - m_rows[2][1] * m_rows[0][2])
        + m_rows[2][0] * (m_rows[0][1] * m_rows[1][2] - m_rows[1][1] * m_rows[0][2]);

      Real det = Real(1.0) / (m_rows[0][0] * c00 - m_rows[1][0] * c10 
        + m_rows[2][0] * c20 - m_rows[3][0] * c30);
      
      return Matrix4(c00 * det, -c10 * det, c20 * det, -c30 * det,
        -c01 * det, c11 * det, -c21 * det, c31 * det,
        c02 * det, -c12 * det, c22 * det, -c32 * det,
        -c03 * det, c13* det, -c23 * det, c33 * det);
#endif
    }

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL Inverse(void)
    {
      *this = Inverted();

      return *this;
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL TransformNormal(const Vector3 &n) const
    {
#ifdef SF_USE_SIMD
      __m128 v = _mm_set_ps(0.0, n[2], n[1], n[0]);
      __m128 mx = _mm_mul_ps(m_rows[0].Get128(), v);
      __m128 w = _mm_shuffle_ps(mx, mx, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
      __m128 z = _mm_shuffle_ps(mx, mx, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
      __m128 y = _mm_shuffle_ps(mx, mx, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

      mx = _mm_add_ps(mx, w);
      mx = _mm_add_ps(mx, z);
      mx = _mm_add_ps(mx, y);

      __m128 my = _mm_mul_ps(m_rows[1].Get128(), v);
      w = _mm_shuffle_ps(my, my, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
      z = _mm_shuffle_ps(my, my, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
      y = _mm_shuffle_ps(my, my, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

      my = _mm_add_ps(my, w);
      my = _mm_add_ps(my, z);
      my = _mm_add_ps(my, y);

      __m128 mz = _mm_mul_ps(m_rows[2].Get128(), v);
      w = _mm_shuffle_ps(mz, mz, 0xFF); //_MM_SHUFFLE(3, 3, 3, 3)
      z = _mm_shuffle_ps(mz, mz, 0xAA); //_MM_SHUFFLE(2, 2, 2, 2)
      y = _mm_shuffle_ps(mz, mz, 0x55); //_MM_SHUFFLE(1, 1, 1, 1)

      mz = _mm_add_ps(mz, w);
      mz = _mm_add_ps(mz, z);
      mz = _mm_add_ps(mz, y);

      return Vector3(_mm_cvtss_f32(mx), _mm_cvtss_f32(my), _mm_cvtss_f32(mz));
#else
      return Vector3(m_rows[0][0] * n[0] + m_rows[0][1] * n[1] + m_rows[0][2] * n[2],
        m_rows[1][0] * n[0] + m_rows[1][1] * n[1] + m_rows[1][2] * n[2],
        m_rows[2][0] * n[0] + m_rows[2][1] * n[1] + m_rows[2][2] * n[2]);
#endif
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL GetColumn(int i) const
    {
      SF_ASSERT(0 <= i && i < 4, "Index is out of range");

      return Vector3(m_rows[0][i], m_rows[1][i], m_rows[2][i]);
    }

    SF_FORCE_INLINE Vector4& SF_VEC_CALL operator[](int i)
    {
      SF_ASSERT(0 <= i && i < 4, "Index is out of range");

      return m_rows[i];
    }

    SF_FORCE_INLINE const Vector4& SF_VEC_CALL operator[](int i) const
    {
      SF_ASSERT(0 <= i && i < 4, "Index is out of range");

      return m_rows[i];
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL operator+(const Matrix4 &other) const
    {
      return Matrix4(m_rows[0] + other.m_rows[0], m_rows[1] + other.m_rows[1],
        m_rows[2] + other.m_rows[2], m_rows[3] + other.m_rows[3]);
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL operator-(const Matrix4 &other) const
    {
      return Matrix4(m_rows[0] - other.m_rows[0], m_rows[1] - other.m_rows[1],
        m_rows[2] - other.m_rows[2], m_rows[3] - other.m_rows[3]);
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL operator*(const Matrix4 &other) const
    {
#ifdef SF_USE_SIMD
      __m128 tv0 = m_rows[0].Get128();
      __m128 tv1 = m_rows[1].Get128();
      __m128 tv2 = m_rows[2].Get128();
      __m128 tv3 = m_rows[3].Get128();
      __m128 mv0 = other.m_rows[0].Get128();
      __m128 mv1 = other.m_rows[1].Get128();
      __m128 mv2 = other.m_rows[2].Get128();
      __m128 mv3 = other.m_rows[3].Get128();

      __m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
      __m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
      __m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);
      __m128 v03 = _mm_shuffle_ps(tv0, tv0, 0xFF);

      v00 = _mm_mul_ps(v00, mv0);
      v01 = _mm_mul_ps(v01, mv1);
      v02 = _mm_mul_ps(v02, mv2);
      v03 = _mm_mul_ps(v03, mv3);

      __m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
      __m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
      __m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);
      __m128 v13 = _mm_shuffle_ps(tv1, tv1, 0xFF);

      v10 = _mm_mul_ps(v10, mv0);
      v11 = _mm_mul_ps(v11, mv1);
      v12 = _mm_mul_ps(v12, mv2);
      v13 = _mm_mul_ps(v13, mv3);

      __m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
      __m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
      __m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);
      __m128 v23 = _mm_shuffle_ps(tv2, tv2, 0xFF);

      v20 = _mm_mul_ps(v20, mv0);
      v21 = _mm_mul_ps(v21, mv1);
      v22 = _mm_mul_ps(v22, mv2);
      v23 = _mm_mul_ps(v23, mv3);

      __m128 v30 = _mm_shuffle_ps(tv3, tv3, 0x0);
      __m128 v31 = _mm_shuffle_ps(tv3, tv3, 0x55);
      __m128 v32 = _mm_shuffle_ps(tv3, tv3, 0xAA);
      __m128 v33 = _mm_shuffle_ps(tv3, tv3, 0xFF);

      v30 = _mm_mul_ps(v30, mv0);
      v31 = _mm_mul_ps(v31, mv1);
      v32 = _mm_mul_ps(v32, mv2);
      v33 = _mm_mul_ps(v33, mv3);

      v00 = _mm_add_ps(v00, v01);
      v02 = _mm_add_ps(v02, v03);
      v00 = _mm_add_ps(v00, v02);

      v10 = _mm_add_ps(v10, v11);
      v12 = _mm_add_ps(v12, v13);
      v10 = _mm_add_ps(v10, v12);

      v20 = _mm_add_ps(v20, v21);
      v22 = _mm_add_ps(v22, v23);
      v20 = _mm_add_ps(v20, v22);

      v30 = _mm_add_ps(v30, v31);
      v32 = _mm_add_ps(v32, v33);
      v30 = _mm_add_ps(v30, v32);

      return Matrix4(v00, v10, v20, v30);
#else
      return Matrix4(m_rows[0].Dot(other._GetColumn4(0)), m_rows[0].Dot(other._GetColumn4(1)),
        m_rows[0].Dot(other._GetColumn4(2)), m_rows[0].Dot(other._GetColumn4(3)),
        m_rows[1].Dot(other._GetColumn4(0)), m_rows[1].Dot(other._GetColumn4(1)),
        m_rows[1].Dot(other._GetColumn4(2)), m_rows[1].Dot(other._GetColumn4(3)),
        m_rows[2].Dot(other._GetColumn4(0)), m_rows[2].Dot(other._GetColumn4(1)),
        m_rows[2].Dot(other._GetColumn4(2)), m_rows[2].Dot(other._GetColumn4(3)),
        m_rows[3].Dot(other._GetColumn4(0)), m_rows[3].Dot(other._GetColumn4(1)),
        m_rows[3].Dot(other._GetColumn4(2)), m_rows[3].Dot(other._GetColumn4(3)));
#endif
    }

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL operator*=(const Matrix4 &other)
    {
#ifdef SF_USE_SIMD
      __m128 tv0 = m_rows[0].Get128();
      __m128 tv1 = m_rows[1].Get128();
      __m128 tv2 = m_rows[2].Get128();
      __m128 tv3 = m_rows[3].Get128();
      __m128 mv0 = other.m_rows[0].Get128();
      __m128 mv1 = other.m_rows[1].Get128();
      __m128 mv2 = other.m_rows[2].Get128();
      __m128 mv3 = other.m_rows[3].Get128();

      __m128 v00 = _mm_shuffle_ps(tv0, tv0, 0x0);
      __m128 v01 = _mm_shuffle_ps(tv0, tv0, 0x55);
      __m128 v02 = _mm_shuffle_ps(tv0, tv0, 0xAA);
      __m128 v03 = _mm_shuffle_ps(tv0, tv0, 0xFF);

      v00 = _mm_mul_ps(v00, mv0);
      v01 = _mm_mul_ps(v01, mv1);
      v02 = _mm_mul_ps(v02, mv2);
      v03 = _mm_mul_ps(v03, mv3);

      __m128 v10 = _mm_shuffle_ps(tv1, tv1, 0x0);
      __m128 v11 = _mm_shuffle_ps(tv1, tv1, 0x55);
      __m128 v12 = _mm_shuffle_ps(tv1, tv1, 0xAA);
      __m128 v13 = _mm_shuffle_ps(tv1, tv1, 0xFF);

      v10 = _mm_mul_ps(v10, mv0);
      v11 = _mm_mul_ps(v11, mv1);
      v12 = _mm_mul_ps(v12, mv2);
      v13 = _mm_mul_ps(v13, mv3);

      __m128 v20 = _mm_shuffle_ps(tv2, tv2, 0x0);
      __m128 v21 = _mm_shuffle_ps(tv2, tv2, 0x55);
      __m128 v22 = _mm_shuffle_ps(tv2, tv2, 0xAA);
      __m128 v23 = _mm_shuffle_ps(tv2, tv2, 0xFF);

      v20 = _mm_mul_ps(v20, mv0);
      v21 = _mm_mul_ps(v21, mv1);
      v22 = _mm_mul_ps(v22, mv2);
      v23 = _mm_mul_ps(v23, mv3);

      __m128 v30 = _mm_shuffle_ps(tv3, tv3, 0x0);
      __m128 v31 = _mm_shuffle_ps(tv3, tv3, 0x55);
      __m128 v32 = _mm_shuffle_ps(tv3, tv3, 0xAA);
      __m128 v33 = _mm_shuffle_ps(tv3, tv3, 0xFF);

      v30 = _mm_mul_ps(v30, mv0);
      v31 = _mm_mul_ps(v31, mv1);
      v32 = _mm_mul_ps(v32, mv2);
      v33 = _mm_mul_ps(v33, mv3);

      v00 = _mm_add_ps(v00, v01);
      v02 = _mm_add_ps(v02, v03);
      m_rows[0].Set(_mm_add_ps(v00, v02));

      v10 = _mm_add_ps(v10, v11);
      v12 = _mm_add_ps(v12, v13);
      m_rows[1].Set(_mm_add_ps(v10, v12));

      v20 = _mm_add_ps(v20, v21);
      v22 = _mm_add_ps(v22, v23);
      m_rows[2].Set(_mm_add_ps(v20, v22));

      v30 = _mm_add_ps(v30, v31);
      v32 = _mm_add_ps(v32, v33);
      m_rows[3].Set(_mm_add_ps(v30, v32));
#else
      m_rows[0].Set(m_rows[0].Dot(other._GetColumn4(0)), m_rows[0].Dot(other._GetColumn4(1)),
        m_rows[0].Dot(other._GetColumn4(2)), m_rows[0].Dot(other._GetColumn4(3)));
      m_rows[1].Set(m_rows[1].Dot(other._GetColumn4(0)), m_rows[1].Dot(other._GetColumn4(1)),
        m_rows[1].Dot(other._GetColumn4(2)), m_rows[1].Dot(other._GetColumn4(3)));
      m_rows[2].Set(m_rows[2].Dot(other._GetColumn4(0)), m_rows[2].Dot(other._GetColumn4(1)),
        m_rows[2].Dot(other._GetColumn4(2)), m_rows[2].Dot(other._GetColumn4(3)));
      m_rows[3].Set(m_rows[3].Dot(other._GetColumn4(0)), m_rows[3].Dot(other._GetColumn4(1)),
        m_rows[3].Dot(other._GetColumn4(2)), m_rows[3].Dot(other._GetColumn4(3)));
#endif

      return *this;
    }

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL operator+=(const Matrix4 &other)
    {
      m_rows[0] += other.m_rows[0];
      m_rows[1] += other.m_rows[1];
      m_rows[2] += other.m_rows[2];
      m_rows[3] += other.m_rows[3];

      return *this;
    }

    SF_FORCE_INLINE Matrix4& SF_VEC_CALL operator-=(const Matrix4 &other)
    {
      m_rows[0] -= other.m_rows[0];
      m_rows[1] -= other.m_rows[1];
      m_rows[2] -= other.m_rows[2];
      m_rows[3] -= other.m_rows[3];

      return *this;
    }

    SF_FORCE_INLINE Matrix4 SF_VEC_CALL operator*(Real s) const
    {
      return Matrix4(m_rows[0] * s, m_rows[1] * s, m_rows[2] * s, m_rows[3] * s);
    }

    SF_FORCE_INLINE Vector3 SF_VEC_CALL operator*(const Vector3 &v) const
    {
      return Vector3(m_rows[0].Dot(v), m_rows[1].Dot(v), m_rows[2].Dot(v));
    }

    SF_FORCE_INLINE Vector4 SF_VEC_CALL operator*(const Vector4 &v) const
    {
      return Vector4(m_rows[0].Dot(v), m_rows[1].Dot(v), m_rows[2].Dot(v), m_rows[3].Dot(v));
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator==(const Matrix4 &other) const
    {
      return (m_rows[0] == other.m_rows[0]) && (m_rows[1] == other.m_rows[1])
        && (m_rows[2] == other.m_rows[2]) && (m_rows[3] == other.m_rows[3]);
    }

    SF_FORCE_INLINE bool SF_VEC_CALL operator!=(const Matrix4 &other) const
    {
      return !(*this == other);
    }

  private:
    SF_FORCE_INLINE Vector4 SF_VEC_CALL _GetColumn4(int i) const
    {
      SF_ASSERT(0 <= i && i < 4, "Index is out of range");
      return Vector4(m_rows[0][i], m_rows[1][i], m_rows[2][i],
        m_rows[3][i]);
    }

    Vector4 m_rows[4];
  };

  SF_FORCE_INLINE void SF_VEC_CALL SetIdentity(Matrix4 &m)
  {
    m.SetIdentity();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL GetDeterminantFull(const Matrix4 &m)
  {
    return m.DeterminantFull();
  }

  SF_FORCE_INLINE Real SF_VEC_CALL GetDeterminantShort(const Matrix4 &m)
  {
    return m.DeterminantShort();
  }

  SF_FORCE_INLINE Matrix4 SF_VEC_CALL GetTranspose(const Matrix4 &m)
  {
    return m.Transposed();
  }

  SF_FORCE_INLINE Matrix4& SF_VEC_CALL Transpose(Matrix4 &m)
  {
    return m.Transpose();
  }

  SF_FORCE_INLINE Matrix4 SF_VEC_CALL GetInverse(const Matrix4 &m)
  {
    return m.Inverted();
  }

  SF_FORCE_INLINE Matrix4& SF_VEC_CALL Inverse(Matrix4 &m)
  {
    return m.Inverse();
  }
}