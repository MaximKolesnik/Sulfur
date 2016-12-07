/******************************************************************************/
/*!
\par     Sulfur
\file    utMatrix4.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Math/sfMathDefs.hpp"
#include "Math/sfMatrix4.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfQuaternion.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

#pragma warning(disable : 4305)

namespace UnitTests
{
  template<typename T>
  static bool AreEqual(const T &expected, const T &actual)
  {
    if (std::abs(expected - actual) < SF_UT_EPSILON)
      return true;

    return false;
  }

  TEST_CLASS(utMatrix4)
  {
    TEST_METHOD(Matrix4Set)
    {
      Matrix4 m(
        1.0, 2.5, 3.1, 4.0,
        5.8, 6.0, 7.0, 8.9,
        9.1, 10.1, 11.2, 12.0,
        13.0, 14.0, 14.6, 15.4
      );

      Assert::AreEqual(Real(1.0), m[0][0]);
      Assert::AreEqual(Real(2.5), m[0][1]);
      Assert::AreEqual(Real(3.1), m[0][2]);
      Assert::AreEqual(Real(4.0), m[0][3]);
      Assert::AreEqual(Real(5.8), m[1][0]);
      Assert::AreEqual(Real(6.0), m[1][1]);
      Assert::AreEqual(Real(7.0), m[1][2]);
      Assert::AreEqual(Real(8.9), m[1][3]);
      Assert::AreEqual(Real(9.1), m[2][0]);
      Assert::AreEqual(Real(10.1), m[2][1]);
      Assert::AreEqual(Real(11.2), m[2][2]);
      Assert::AreEqual(Real(12.0), m[2][3]);
      Assert::AreEqual(Real(13.0), m[3][0]);
      Assert::AreEqual(Real(14.0), m[3][1]);
      Assert::AreEqual(Real(14.6), m[3][2]);
      Assert::AreEqual(Real(15.4), m[3][3]);

      Matrix4 mo(m);

      Assert::AreEqual(mo[0][0], m[0][0]);
      Assert::AreEqual(mo[0][1], m[0][1]);
      Assert::AreEqual(mo[0][2], m[0][2]);
      Assert::AreEqual(mo[0][3], m[0][3]);
      Assert::AreEqual(mo[1][0], m[1][0]);
      Assert::AreEqual(mo[1][1], m[1][1]);
      Assert::AreEqual(mo[1][2], m[1][2]);
      Assert::AreEqual(mo[1][3], m[1][3]);
      Assert::AreEqual(mo[2][0], m[2][0]);
      Assert::AreEqual(mo[2][1], m[2][1]);
      Assert::AreEqual(mo[2][2], m[2][2]);
      Assert::AreEqual(mo[2][3], m[2][3]);
      Assert::AreEqual(mo[3][0], m[3][0]);
      Assert::AreEqual(mo[3][1], m[3][1]);
      Assert::AreEqual(mo[3][2], m[3][2]);
      Assert::AreEqual(mo[3][3], m[3][3]);

      Vector4 v0(1.0, 2.5, 3.1, 4.0), v1(5.8, 6.0, 7.0, 8.9),
        v2(9.1, 10.1, 11.2, 12.0), v3(13.0, 14.0, 14.6, 15.4);
      Matrix4 mv(v0, v1, v2, v3);

      Assert::AreEqual(v0[0], mv[0][0]);
      Assert::AreEqual(v0[1], mv[0][1]);
      Assert::AreEqual(v0[2], mv[0][2]);
      Assert::AreEqual(v0[3], mv[0][3]);
      Assert::AreEqual(v1[0], mv[1][0]);
      Assert::AreEqual(v1[1], mv[1][1]);
      Assert::AreEqual(v1[2], mv[1][2]);
      Assert::AreEqual(v1[3], mv[1][3]);
      Assert::AreEqual(v2[0], mv[2][0]);
      Assert::AreEqual(v2[1], mv[2][1]);
      Assert::AreEqual(v2[2], mv[2][2]);
      Assert::AreEqual(v2[3], mv[2][3]);
      Assert::AreEqual(v3[0], mv[3][0]);
      Assert::AreEqual(v3[1], mv[3][1]);
      Assert::AreEqual(v3[2], mv[3][2]);
      Assert::AreEqual(v3[3], mv[3][3]);

      Matrix4 ms;
      ms.Set(
        11.4, 23.1, 556.3, 42.1,
        432.1, 32.6, 43.1, 75.23,
        12.1, 442.1, 98.6, 464.3,
        34.5, 234.1, 453.2, -0.4
      );

      Assert::AreEqual(Real(11.4), ms[0][0]);
      Assert::AreEqual(Real(23.1), ms[0][1]);
      Assert::AreEqual(Real(556.3), ms[0][2]);
      Assert::AreEqual(Real(42.1), ms[0][3]);
      Assert::AreEqual(Real(432.1), ms[1][0]);
      Assert::AreEqual(Real(32.6), ms[1][1]);
      Assert::AreEqual(Real(43.1), ms[1][2]);
      Assert::AreEqual(Real(75.23), ms[1][3]);
      Assert::AreEqual(Real(12.1), ms[2][0]);
      Assert::AreEqual(Real(442.1), ms[2][1]);
      Assert::AreEqual(Real(98.6), ms[2][2]);
      Assert::AreEqual(Real(464.3), ms[2][3]);
      Assert::AreEqual(Real(34.5), ms[3][0]);
      Assert::AreEqual(Real(234.1), ms[3][1]);
      Assert::AreEqual(Real(453.2), ms[3][2]);
      Assert::AreEqual(Real(-0.4), ms[3][3]);
    }

    TEST_METHOD(Matrix4OpEq)
    {
      Matrix4 ms, mr;
      ms.Set(
        11.4, 23.1, 556.3, 42.1,
        432.1, 32.6, 43.1, 75.23,
        12.1, 442.1, 98.6, 464.3,
        34.5, 234.1, 453.2, -0.4
      );

      mr = ms;

      Assert::AreEqual(mr[0][0], ms[0][0]);
      Assert::AreEqual(mr[0][1], ms[0][1]);
      Assert::AreEqual(mr[0][2], ms[0][2]);
      Assert::AreEqual(mr[0][3], ms[0][3]);
      Assert::AreEqual(mr[1][0], ms[1][0]);
      Assert::AreEqual(mr[1][1], ms[1][1]);
      Assert::AreEqual(mr[1][2], ms[1][2]);
      Assert::AreEqual(mr[1][3], ms[1][3]);
      Assert::AreEqual(mr[2][0], ms[2][0]);
      Assert::AreEqual(mr[2][1], ms[2][1]);
      Assert::AreEqual(mr[2][2], ms[2][2]);
      Assert::AreEqual(mr[2][3], ms[2][3]);
      Assert::AreEqual(mr[3][0], ms[3][0]);
      Assert::AreEqual(mr[3][1], ms[3][1]);
      Assert::AreEqual(mr[3][2], ms[3][2]);
      Assert::AreEqual(mr[3][3], ms[3][3]);
    }

    TEST_METHOD(Matrix4SetIdentity)
    {
      Matrix4 mi;
      mi.Set(
        11.4, 23.1, 556.3, 42.1,
        432.1, 32.6, 43.1, 75.23,
        12.1, 442.1, 98.6, 464.3,
        34.5, 234.1, 453.2, -0.4
      );

      mi.SetIdentity();

      Assert::AreEqual(Real(1.0), mi[0][0]);
      Assert::AreEqual(Real(0.0), mi[0][1]);
      Assert::AreEqual(Real(0.0), mi[0][2]);
      Assert::AreEqual(Real(0.0), mi[0][3]);
      Assert::AreEqual(Real(0.0), mi[1][0]);
      Assert::AreEqual(Real(1.0), mi[1][1]);
      Assert::AreEqual(Real(0.0), mi[1][2]);
      Assert::AreEqual(Real(0.0), mi[1][3]);
      Assert::AreEqual(Real(0.0), mi[2][0]);
      Assert::AreEqual(Real(0.0), mi[2][1]);
      Assert::AreEqual(Real(1.0), mi[2][2]);
      Assert::AreEqual(Real(0.0), mi[2][3]);
      Assert::AreEqual(Real(0.0), mi[3][0]);
      Assert::AreEqual(Real(0.0), mi[3][1]);
      Assert::AreEqual(Real(0.0), mi[3][2]);
      Assert::AreEqual(Real(1.0), mi[3][3]);
    }

    TEST_METHOD(Matrix4SetRotation)
    {
      Matrix4 mRotX, mRotY, mRotZ;

      mRotX.SetRotationDeg(30.0, 0.0, 0.0);
      mRotY.SetRotationDeg(0.0, 45.0, 0.0);
      mRotZ.SetRotationDeg(0.0, 0.0, 70.0);

      Matrix4 mqX, mqY, mqZ;
      Quaternion q;

      q.Set(Vector3::c_xAxis, 30.0 * SF_RADS_PER_DEG);
      mqX.SetRotation(q);
      q.Set(Vector3::c_zAxis, 45 * SF_RADS_PER_DEG);
      mqY.SetRotation(q);
      q.Set(Vector3::c_yAxis, 70 * SF_RADS_PER_DEG);
      mqZ.SetRotation(q);

      Assert::IsTrue(AreEqual(mRotX[0][0], mqX[0][0]));
      Assert::IsTrue(AreEqual(mRotX[0][1], mqX[0][1]));
      Assert::IsTrue(AreEqual(mRotX[0][2], mqX[0][2]));
      Assert::IsTrue(AreEqual(mRotX[0][3], mqX[0][3]));
      Assert::IsTrue(AreEqual(mRotX[1][0], mqX[1][0]));
      Assert::IsTrue(AreEqual(mRotX[1][1], mqX[1][1]));
      Assert::IsTrue(AreEqual(mRotX[1][2], mqX[1][2]));
      Assert::IsTrue(AreEqual(mRotX[1][3], mqX[1][3]));
      Assert::IsTrue(AreEqual(mRotX[2][0], mqX[2][0]));
      Assert::IsTrue(AreEqual(mRotX[2][1], mqX[2][1]));
      Assert::IsTrue(AreEqual(mRotX[2][2], mqX[2][2]));
      Assert::IsTrue(AreEqual(mRotX[2][3], mqX[2][3]));
      Assert::IsTrue(AreEqual(mRotX[3][0], mqX[3][0]));
      Assert::IsTrue(AreEqual(mRotX[3][1], mqX[3][1]));
      Assert::IsTrue(AreEqual(mRotX[3][2], mqX[3][2]));
      Assert::IsTrue(AreEqual(mRotX[3][3], mqX[3][3]));

      Assert::IsTrue(AreEqual(mRotY[0][0], mqY[0][0]));
      Assert::IsTrue(AreEqual(mRotY[0][1], mqY[0][1]));
      Assert::IsTrue(AreEqual(mRotY[0][2], mqY[0][2]));
      Assert::IsTrue(AreEqual(mRotY[0][3], mqY[0][3]));
      Assert::IsTrue(AreEqual(mRotY[1][0], mqY[1][0]));
      Assert::IsTrue(AreEqual(mRotY[1][1], mqY[1][1]));
      Assert::IsTrue(AreEqual(mRotY[1][2], mqY[1][2]));
      Assert::IsTrue(AreEqual(mRotY[1][3], mqY[1][3]));
      Assert::IsTrue(AreEqual(mRotY[2][0], mqY[2][0]));
      Assert::IsTrue(AreEqual(mRotY[2][1], mqY[2][1]));
      Assert::IsTrue(AreEqual(mRotY[2][2], mqY[2][2]));
      Assert::IsTrue(AreEqual(mRotY[2][3], mqY[2][3]));
      Assert::IsTrue(AreEqual(mRotY[3][0], mqY[3][0]));
      Assert::IsTrue(AreEqual(mRotY[3][1], mqY[3][1]));
      Assert::IsTrue(AreEqual(mRotY[3][2], mqY[3][2]));
      Assert::IsTrue(AreEqual(mRotY[3][3], mqY[3][3]));

      Assert::IsTrue(AreEqual(mRotZ[0][0], mqZ[0][0]));
      Assert::IsTrue(AreEqual(mRotZ[0][1], mqZ[0][1]));
      Assert::IsTrue(AreEqual(mRotZ[0][2], mqZ[0][2]));
      Assert::IsTrue(AreEqual(mRotZ[0][3], mqZ[0][3]));
      Assert::IsTrue(AreEqual(mRotZ[1][0], mqZ[1][0]));
      Assert::IsTrue(AreEqual(mRotZ[1][1], mqZ[1][1]));
      Assert::IsTrue(AreEqual(mRotZ[1][2], mqZ[1][2]));
      Assert::IsTrue(AreEqual(mRotZ[1][3], mqZ[1][3]));
      Assert::IsTrue(AreEqual(mRotZ[2][0], mqZ[2][0]));
      Assert::IsTrue(AreEqual(mRotZ[2][1], mqZ[2][1]));
      Assert::IsTrue(AreEqual(mRotZ[2][2], mqZ[2][2]));
      Assert::IsTrue(AreEqual(mRotZ[2][3], mqZ[2][3]));
      Assert::IsTrue(AreEqual(mRotZ[3][0], mqZ[3][0]));
      Assert::IsTrue(AreEqual(mRotZ[3][1], mqZ[3][1]));
      Assert::IsTrue(AreEqual(mRotZ[3][2], mqZ[3][2]));
      Assert::IsTrue(AreEqual(mRotZ[3][3], mqZ[3][3]));
    }

    TEST_METHOD(Matrix4GetQuat)
    {
      Matrix4 m1, m2, m3;
      m1.SetRotationDeg(0, -90, 90);
      m2.SetRotationDeg(45.0, 45.0, 45.0);

      Quaternion q1;
      q1 = m1.GetQuaternion();
      Assert::IsTrue(AreEqual(Real(-0.5), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.5), q1[1]));
      Assert::IsTrue(AreEqual(Real(-0.5), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.5), q1[3]));

      Quaternion q2;
      q2 = m2.GetQuaternion();
      Assert::IsTrue(AreEqual(Real(0.732537), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.461939), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.461939), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.191341), q2[3]));
    }

    TEST_METHOD(Matrix4DetFull)
    {
      Matrix4 m1, m2;
      m1.Set(
        11.4, 23.1, 556.3, 42.1,
        432.1, 32.6, 43.1, 75.23,
        12.1, 442.1, 98.6, 464.3,
        34.5, 234.1, 453.2, -0.4
      );

#ifdef SF_USE_SIMD
      Assert::IsTrue(AreEqual(Real(-27390011289), m1.DeterminantFull()));
#endif

      m2.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );

      Assert::IsTrue(AreEqual(Real(-20863.6777), m2.DeterminantFull()));
    }

    TEST_METHOD(Matrix4SetScaling)
    {
      Matrix4 m1, m2;
      m1.SetScaling(3.0, -1.0, 5.0);

      Assert::IsTrue(AreEqual(Real(3.0), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(-1.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(5.0), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[3][3]));

      m2.SetScaling(-2.4, 4.87, 25.23);

      Assert::IsTrue(AreEqual(Real(-2.4), m2[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[0][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[1][0]));
      Assert::IsTrue(AreEqual(Real(4.87), m2[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[2][1]));
      Assert::IsTrue(AreEqual(Real(25.23), m2[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m2[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m2[3][3]));
    }

    TEST_METHOD(Matrix4SetTranslation)
    {
      Matrix4 m1;

      m1.SetTranslation(3.0, 6.0, 120.0);
      Assert::IsTrue(AreEqual(Real(1.0), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(3.0), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(6.0), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(120.0), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[3][3]));

      m1.SetTranslation(-121.876, 44.44, 67.789);
      Assert::IsTrue(AreEqual(Real(1.0), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-121.876), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(44.44), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(67.789), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[3][3]));
    }

    TEST_METHOD(Matrix4SetTransform)
    {
      Matrix4 m1;

      m1.SetTransformation(Vector3(SF_PI / 2, SF_PI / 2, 0.0), Vector3(1, 2, 3),
        Vector3(-3.0, 45.0, 15.0));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(3.0), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-3.0), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(45.0), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(2.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(15.0), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[3][3]));
    }

    TEST_METHOD(Matrix4DetShort)
    {
      Matrix4 m;

      m.Set(
        1.5, 2.3, 4.1, 5.0,
        -20.1, 7.2, 3.1, 10.1,
        15.2, 31.2, -6.5, -3.2,
        0, 0, 0, 1
      );

      Assert::IsTrue(AreEqual(Real(-3427.29541), m.DeterminantShort()));
    }

    TEST_METHOD(Matrix4Transpose)
    {
      Matrix4 m, mr;
      m.Set(
        1.2, 2.3, 3.1, 4.6,
        5.2, 6.7, 7.9, 8.12,
        9.1, 10.21, 11.12, 12.9,
        13.3, 14.2, 15.55, 16.23
      );

      mr = m.Transposed();
      Assert::IsTrue(AreEqual(Real(1.2), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(5.2), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(9.1), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(13.3), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(2.3), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(6.7), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(10.21), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(14.2), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(3.1), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(7.9), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(11.12), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(15.55), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(4.6), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(8.12), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(12.9), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(16.23), mr[3][3]));

      m.Transpose();
      Assert::IsTrue(AreEqual(Real(1.2), m[0][0]));
      Assert::IsTrue(AreEqual(Real(5.2), m[0][1]));
      Assert::IsTrue(AreEqual(Real(9.1), m[0][2]));
      Assert::IsTrue(AreEqual(Real(13.3), m[0][3]));
      Assert::IsTrue(AreEqual(Real(2.3), m[1][0]));
      Assert::IsTrue(AreEqual(Real(6.7), m[1][1]));
      Assert::IsTrue(AreEqual(Real(10.21), m[1][2]));
      Assert::IsTrue(AreEqual(Real(14.2), m[1][3]));
      Assert::IsTrue(AreEqual(Real(3.1), m[2][0]));
      Assert::IsTrue(AreEqual(Real(7.9), m[2][1]));
      Assert::IsTrue(AreEqual(Real(11.12), m[2][2]));
      Assert::IsTrue(AreEqual(Real(15.55), m[2][3]));
      Assert::IsTrue(AreEqual(Real(4.6), m[3][0]));
      Assert::IsTrue(AreEqual(Real(8.12), m[3][1]));
      Assert::IsTrue(AreEqual(Real(12.9), m[3][2]));
      Assert::IsTrue(AreEqual(Real(16.23), m[3][3]));
    }

    TEST_METHOD(Matrix4GetCofactorMatrix)
    {
      Matrix4 m, mr;
      m.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );

      mr = m.GetCofactorMatrix();
      Assert::IsTrue(AreEqual(Real(-5449.42676), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(3379.65820), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(-830.549927), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-686.249939), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(1201.03796), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-1135.61401), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(-977.130005), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(247.689987), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(-1496.93994), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(457.539978), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(-338.260010), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(-1033.09998), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(2468.07007), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(-106.269928), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(-546.150024), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(490.750000), mr[3][3]));
    }

    TEST_METHOD(Matrix4GetAdjointMatrix)
    {
      Matrix4 m, mr;
      m.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );

      mr = m.GetAdjointMatrix();
      Assert::IsTrue(AreEqual(Real(-5449.42676), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(1201.03796), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(-1496.93994), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(2468.07007), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(3379.65820), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-1135.61401), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(457.539978), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(-106.269928), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(-830.549927), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-977.130005), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(-338.260010), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(-546.150024), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(-686.249939), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(247.689987), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(-1033.09998), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(490.750000), mr[3][3]));
    }

    TEST_METHOD(Matrix4Inverse)
    {
      Matrix4 m, mr;
      m.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );

      Real det = m.DeterminantFull();

      mr = m.Inverted();
      Assert::IsTrue(AreEqual(Real(0.2611920353), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-0.057565982), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0717486218), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-0.118295069), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(-0.161987657), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0544301972), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(-0.021929980), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0050935415), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0398084211), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0468340286), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0162128668), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0261770744), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0328920943), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(-0.011871829), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0495166815), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(-0.023521741), mr[3][3]));

      m.Inverse();
      Assert::IsTrue(AreEqual(Real(0.2611920353), m[0][0]));
      Assert::IsTrue(AreEqual(Real(-0.057565982), m[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0717486218), m[0][2]));
      Assert::IsTrue(AreEqual(Real(-0.118295069), m[0][3]));
      Assert::IsTrue(AreEqual(Real(-0.161987657), m[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0544301972), m[1][1]));
      Assert::IsTrue(AreEqual(Real(-0.021929980), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0050935415), m[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0398084211), m[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0468340286), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0162128668), m[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0261770744), m[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0328920943), m[3][0]));
      Assert::IsTrue(AreEqual(Real(-0.011871829), m[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0495166815), m[3][2]));
      Assert::IsTrue(AreEqual(Real(-0.023521741), m[3][3]));
    }

    TEST_METHOD(Matrix4GetColumn)
    {
      Matrix4 m;
      m.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      Vector3 c;

      c = m.GetColumn(0);
      Assert::AreEqual(c[0], m[0][0]);
      Assert::AreEqual(c[1], m[1][0]);
      Assert::AreEqual(c[2], m[2][0]);

      c = m.GetColumn(1);
      Assert::AreEqual(c[0], m[0][1]);
      Assert::AreEqual(c[1], m[1][1]);
      Assert::AreEqual(c[2], m[2][1]);

      c = m.GetColumn(2);
      Assert::AreEqual(c[0], m[0][2]);
      Assert::AreEqual(c[1], m[1][2]);
      Assert::AreEqual(c[2], m[2][2]);

      c = m.GetColumn(3);
      Assert::AreEqual(c[0], m[0][3]);
      Assert::AreEqual(c[1], m[1][3]);
      Assert::AreEqual(c[2], m[2][3]);
    }

    TEST_METHOD(Matrix4OpAdd)
    {
      Matrix4 m1, m2, mr;
      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        9.0, 20.4, -5.0, -6.0
      );

      mr = m1 + m2;
      Assert::IsTrue(AreEqual(Real(6.099999), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(2.5), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(1.299999), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(0.3), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(-10.0), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-5.0), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(7.0), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(0.599999), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(0.5), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(0.599999), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(0.2), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(-0.199999809), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(8.0), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(-0.3), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(0.3), mr[3][3]));

      m1 += m2;

      Assert::IsTrue(AreEqual(Real(6.099999), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(2.5), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(1.299999), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(0.3), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(-10.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(-5.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(7.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(0.599999), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(0.5), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(0.599999), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(0.2), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(-0.199999809), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(8.0), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(-0.3), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(0.3), m1[3][3]));
    }

    TEST_METHOD(Matrix4OpSub)
    {
      Matrix4 m1, m2, mr;
      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        9.0, 20.4, -5.0, -6.0
      );

      mr = m1 - m2;
      Assert::IsTrue(AreEqual(Real(-2.1), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-8.5), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(7.7), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-11.7), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(20.0), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(25.0), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(21.0), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(-15.4), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(-9.5), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-3.0), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(4.6), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(50.2), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(-18.2), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(-32.8), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(9.7), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(12.3), mr[3][3]));

      m1 -= m2;

      Assert::IsTrue(AreEqual(Real(-2.1), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(-8.5), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(7.7), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-11.7), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(20.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(25.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(21.0), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(-15.4), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(-9.5), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(-3.0), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(4.6), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(50.2), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(-18.2), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(-32.8), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(9.7), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(12.3), m1[3][3]));
    }

    TEST_METHOD(Matrix4OpMul)
    {
      Matrix4 m1, m2, mr;
      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        9.0, 20.4, -5.0, -6.0
      );

      mr = m1 * m2;
      Assert::IsTrue(AreEqual(Real(24.400001), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-53.52999), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(34.099998), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-90.300003), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(-126.099998), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-252.460007), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(-77), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(-195.600006), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(243.850006), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(515.730042), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(-106.299995), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(-255.200012), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(228.479996), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(270.970001), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(75.3399963), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(-309.699982), mr[3][3]));

      m1 *= m2;

      Assert::IsTrue(AreEqual(Real(24.400001), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(-53.52999), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(34.099998), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-90.300003), m1[0][3]));
      Assert::IsTrue(AreEqual(Real(-126.099998), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(-252.460007), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(-77), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(-195.600006), m1[1][3]));
      Assert::IsTrue(AreEqual(Real(243.850006), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(515.730042), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(-106.299995), m1[2][2]));
      Assert::IsTrue(AreEqual(Real(-255.200012), m1[2][3]));
      Assert::IsTrue(AreEqual(Real(228.479996), m1[3][0]));
      Assert::IsTrue(AreEqual(Real(270.970001), m1[3][1]));
      Assert::IsTrue(AreEqual(Real(75.3399963), m1[3][2]));
      Assert::IsTrue(AreEqual(Real(-309.699982), m1[3][3]));
    }

    TEST_METHOD(Matrix4OpScale)
    {
      Matrix4 m1, m2, mr;
      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        9.0, 20.4, -5.0, -6.0
      );

      Real s = 2.0;

      mr = m1 * s;
      Assert::IsTrue(AreEqual(Real(4.0), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-6.0), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(9.0), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-11.3999996), mr[0][3]));
      Assert::IsTrue(AreEqual(Real(10.0), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(20.0), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(28.0), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(-14.8), mr[1][3]));
      Assert::IsTrue(AreEqual(Real(-9.0), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-3.0), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(5.199999), mr[2][2]));
      Assert::IsTrue(AreEqual(Real(50.4), mr[2][3]));
      Assert::IsTrue(AreEqual(Real(-18.4), mr[3][0]));
      Assert::IsTrue(AreEqual(Real(-24.8), mr[3][1]));
      Assert::IsTrue(AreEqual(Real(9.4), mr[3][2]));
      Assert::IsTrue(AreEqual(Real(12.6), mr[3][3]));
    }

    TEST_METHOD(Matrix4MulVec3)
    {
      Matrix4 m;
      m.Set(
        2.0, 3.0, 4.0, 1.0,
        -3.4, 0.5, -2.2, 10.0,
        4.5, -10.5, 4.2, -5.0,
        0, 0, 0, 1
      );

      Vector3 v, vr;
      v.Set(2.0, 3.0, -4.2);

      vr = m * v;

      Assert::IsTrue(AreEqual(Real(-2.8), vr[0]));
      Assert::IsTrue(AreEqual(Real(13.94), vr[1]));
      Assert::IsTrue(AreEqual(Real(-45.14), vr[2]));
    }

    TEST_METHOD(Matrix4EqOps)
    {
      Matrix4 m1, m2, m3;
      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );
      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        9.0, 20.4, -5.0, -6.0
      );
      m3.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
      );

      Assert::IsTrue(m1 == m3);
      Assert::IsFalse(m1 != m3);

      Assert::IsTrue(m1 != m2);
      Assert::IsFalse(m1 == m2);
    }

    TEST_METHOD(Matrix4TransformNormal)
    {
      Matrix4 m1, m2;

      m1.Set(
        2.0, -3.0, 4.5, -5.7,
        5.0, 10.0, 14.0, -7.4,
        -4.5, -1.5, 2.6, 25.2,
        -9.2, -12.4, 4.7, 6.3
        );

      m2.Set(
        4.1, 5.5, -3.2, 6.0,
        -15.0, -15.0, -7.0, 8.0,
        5.0, 1.5, -2.0, -25.0,
        0.0, 0.0, 0.0, 1.0
        );
      
      Vector3 r1(3.1, -2.0, 15.2);
      r1 = m1.TransformNormal(r1);

      Assert::IsTrue(AreEqual(Real(80.6), r1[0]));
      Assert::IsTrue(AreEqual(Real(208.3), r1[1]));
      Assert::IsTrue(AreEqual(Real(28.5699959), r1[2]));

      Vector3 r2(-124.54, 22.1, 51.98);
      r2 = m2.TransformNormal(r2);

      Assert::IsTrue(AreEqual(Real(-555.399963), r2[0]));
      Assert::IsTrue(AreEqual(Real(1172.73999), r2[1]));
      Assert::IsTrue(AreEqual(Real(-693.510010), r2[2]));
    }
  };
}

#pragma warning(default : 4305)