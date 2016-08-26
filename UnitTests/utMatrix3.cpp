#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/Engine/Math/sfMathDefs.hpp"
#include "../src/Engine/Math/sfMatrix3.hpp"
#include "../src/Engine/Math/sfVector3.hpp"
#include "../src/Engine/Math/sfQuaternion.hpp"

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

  TEST_CLASS(utMatrix3)
  {
  public:
    TEST_METHOD(Matrix3Set0)
    {
      Matrix3 m(-0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);

      Assert::AreEqual(Real(-0.1), m[0][0]);
      Assert::AreEqual(Real(3.12), m[0][1]);
      Assert::AreEqual(Real(5.32), m[0][2]);
      Assert::AreEqual(Real(2.6), m[1][0]);
      Assert::AreEqual(Real(-12.41), m[1][1]);
      Assert::AreEqual(Real(3.768), m[1][2]);
      Assert::AreEqual(Real(8.143), m[2][0]);
      Assert::AreEqual(Real(1.329), m[2][1]);
      Assert::AreEqual(Real(72.12), m[2][2]);

      Matrix3 m1(m);

      Assert::AreEqual(Real(-0.1), m1[0][0]);
      Assert::AreEqual(Real(3.12), m1[0][1]);
      Assert::AreEqual(Real(5.32), m1[0][2]);
      Assert::AreEqual(Real(2.6), m1[1][0]);
      Assert::AreEqual(Real(-12.41), m1[1][1]);
      Assert::AreEqual(Real(3.768), m1[1][2]);
      Assert::AreEqual(Real(8.143), m1[2][0]);
      Assert::AreEqual(Real(1.329), m1[2][1]);
      Assert::AreEqual(Real(72.12), m1[2][2]);
    }

    TEST_METHOD(Matrix3Set1)
    {
      Matrix3 m;
      m.Set(
        -2.342, 10.456, 3.11,
        4.123, -94.241, 15.231,
        -0.4123, 9.413, 2.765
      );

      Assert::AreEqual(Real(-2.342), m[0][0]);
      Assert::AreEqual(Real(10.456), m[0][1]);
      Assert::AreEqual(Real(3.11), m[0][2]);
      Assert::AreEqual(Real(4.123), m[1][0]);
      Assert::AreEqual(Real(-94.241), m[1][1]);
      Assert::AreEqual(Real(15.231), m[1][2]);
      Assert::AreEqual(Real(-0.4123), m[2][0]);
      Assert::AreEqual(Real(9.413), m[2][1]);
      Assert::AreEqual(Real(2.765), m[2][2]);

      Matrix3 m1;
      m1 = m;

      Assert::AreEqual(Real(-2.342), m1[0][0]);
      Assert::AreEqual(Real(10.456), m1[0][1]);
      Assert::AreEqual(Real(3.11), m1[0][2]);
      Assert::AreEqual(Real(4.123), m1[1][0]);
      Assert::AreEqual(Real(-94.241), m1[1][1]);
      Assert::AreEqual(Real(15.231), m1[1][2]);
      Assert::AreEqual(Real(-0.4123), m1[2][0]);
      Assert::AreEqual(Real(9.413), m1[2][1]);
      Assert::AreEqual(Real(2.765), m1[2][2]);

      Matrix3 m2(Vector3(-2.342, 4.123, -0.4123),
        Vector3(10.456, -94.241, 9.413),
        Vector3(3.11, 15.231, 2.765));

      Assert::AreEqual(Real(-2.342), m2[0][0]);
      Assert::AreEqual(Real(4.123), m2[0][1]);
      Assert::AreEqual(Real(-0.4123), m2[0][2]);
      Assert::AreEqual(Real(10.456), m2[1][0]);
      Assert::AreEqual(Real(-94.241), m2[1][1]);
      Assert::AreEqual(Real(9.413), m2[1][2]);
      Assert::AreEqual(Real(3.11), m2[2][0]);
      Assert::AreEqual(Real(15.231), m2[2][1]);
      Assert::AreEqual(Real(2.765), m2[2][2]);
    }

    TEST_METHOD(Matrx3Identity)
    {
      Matrix3 m;
      m.SetIdentity();

      Assert::AreEqual(Real(1.0), m[0][0]);
      Assert::AreEqual(Real(0.0), m[0][1]);
      Assert::AreEqual(Real(0.0), m[0][2]);
      Assert::AreEqual(Real(0.0), m[1][0]);
      Assert::AreEqual(Real(1.0), m[1][1]);
      Assert::AreEqual(Real(0.0), m[1][2]);
      Assert::AreEqual(Real(0.0), m[2][0]);
      Assert::AreEqual(Real(0.0), m[2][1]);
      Assert::AreEqual(Real(1.0), m[2][2]);
    }

    TEST_METHOD(Matrix3Rotation)
    {
      Matrix3 m;

      m.SetRotationDeg(90.0, 90.0, -90.0);

      Assert::IsTrue(AreEqual(Real(-1.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));

      m.SetRotationRad(SF_PI / 2, SF_PI / 2, -SF_PI / 2);
      Assert::IsTrue(AreEqual(Real(-1.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));

      Matrix3 mq;
      mq.SetRotationDeg(90.0, 0.0, 0.0);

      Quaternion q;
      q.Set(Vector3(1.0, 0.0, 0.0), SF_PI / 2);
      m.SetRotation(q);

      Assert::IsTrue(AreEqual(mq[0][0], m[0][0]));
      Assert::IsTrue(AreEqual(mq[0][1], m[0][1]));
      Assert::IsTrue(AreEqual(mq[0][2], m[0][2]));
      Assert::IsTrue(AreEqual(mq[1][0], m[1][0]));
      Assert::IsTrue(AreEqual(mq[1][1], m[1][1]));
      Assert::IsTrue(AreEqual(mq[1][2], m[1][2]));
      Assert::IsTrue(AreEqual(mq[2][0], m[2][0]));
      Assert::IsTrue(AreEqual(mq[2][1], m[2][1]));
      Assert::IsTrue(AreEqual(mq[2][2], m[2][2]));
    }

    TEST_METHOD(Matrix3GetQuat)
    {
      Matrix3 m;
      m.SetRotationDeg(90.0, 0.0, 0.0);

      Quaternion q = m.GetQuaternion();

      Assert::IsTrue(AreEqual(Real(0.7071067), q[0]));
      Assert::IsTrue(AreEqual(Real(0.7071067), q[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q[3]));
    }

    TEST_METHOD(Matrix3Det)
    {
      Matrix3 m;
      m.SetIdentity();

      Assert::AreEqual(Real(1.0), m.Determinant());

      m.Set(
        5.0, -10.2, 22.0,
        11.0, 0.0, 4.3,
        16.16, -15.0, 20.5
      );

      Assert::IsTrue(AreEqual(Real(-1716.17773), m.Determinant()));
      
      m.Set(
        5.0, -10.2, 22.0,
        23.0, 11.3, 6.0,
        0.0, 0.0, 0.0
      );

      Assert::AreEqual(Real(0.0), m.Determinant());
    }

    TEST_METHOD(Matrix3GetColumn)
    {
      Matrix3 m;
      m.SetIdentity();

      Vector3 v = m.GetColumn(0);
      Assert::AreEqual(Real(1.0), v[0]);
      Assert::AreEqual(Real(0.0), v[1]);
      Assert::AreEqual(Real(0.0), v[2]);

      v = m.GetColumn(1);
      Assert::AreEqual(Real(0.0), v[0]);
      Assert::AreEqual(Real(1.0), v[1]);
      Assert::AreEqual(Real(0.0), v[2]);

      v = m.GetColumn(2);
      Assert::AreEqual(Real(0.0), v[0]);
      Assert::AreEqual(Real(0.0), v[1]);
      Assert::AreEqual(Real(1.0), v[2]);

      m.Set(
        0.0, 1.0, 2.0,
        3.0, 4.2, 5.3,
        6.4, 7.5, 8.6
      );

      v = m.GetColumn(0);
      Assert::AreEqual(Real(0.0), v[0]);
      Assert::AreEqual(Real(3.0), v[1]);
      Assert::AreEqual(Real(6.4), v[2]);

      v = m.GetColumn(1);
      Assert::AreEqual(Real(1.0), v[0]);
      Assert::AreEqual(Real(4.2), v[1]);
      Assert::AreEqual(Real(7.5), v[2]);

      v = m.GetColumn(2);
      Assert::AreEqual(Real(2.0), v[0]);
      Assert::AreEqual(Real(5.3), v[1]);
      Assert::AreEqual(Real(8.6), v[2]);
    }

    TEST_METHOD(Matrix3Transpose)
    {
      Matrix3 m;
      m.SetIdentity();

      Matrix3 mt = m.Transposed();
      Assert::AreEqual(Real(1.0), mt[0][0]);
      Assert::AreEqual(Real(0.0), mt[0][1]);
      Assert::AreEqual(Real(0.0), mt[0][2]);
      Assert::AreEqual(Real(0.0), mt[1][0]);
      Assert::AreEqual(Real(1.0), mt[1][1]);
      Assert::AreEqual(Real(0.0), mt[1][2]);
      Assert::AreEqual(Real(0.0), mt[2][0]);
      Assert::AreEqual(Real(0.0), mt[2][1]);
      Assert::AreEqual(Real(1.0), mt[2][2]);

      m.Transpose();
      Assert::AreEqual(Real(1.0), m[0][0]);
      Assert::AreEqual(Real(0.0), m[0][1]);
      Assert::AreEqual(Real(0.0), m[0][2]);
      Assert::AreEqual(Real(0.0), m[1][0]);
      Assert::AreEqual(Real(1.0), m[1][1]);
      Assert::AreEqual(Real(0.0), m[1][2]);
      Assert::AreEqual(Real(0.0), m[2][0]);
      Assert::AreEqual(Real(0.0), m[2][1]);
      Assert::AreEqual(Real(1.0), m[2][2]);

      m.Set(
        1.0, 2.2, 3.0,
        4.22, 5.1, 6.7,
        7.89, 8.32, 9.21
      );
      mt = m.Transposed();
      Assert::AreEqual(Real(1.0), mt[0][0]);
      Assert::AreEqual(Real(4.22), mt[0][1]);
      Assert::AreEqual(Real(7.89), mt[0][2]);
      Assert::AreEqual(Real(2.2), mt[1][0]);
      Assert::AreEqual(Real(5.1), mt[1][1]);
      Assert::AreEqual(Real(8.32), mt[1][2]);
      Assert::AreEqual(Real(3.0), mt[2][0]);
      Assert::AreEqual(Real(6.7), mt[2][1]);
      Assert::AreEqual(Real(9.21), mt[2][2]);

      m.Transpose();
      Assert::AreEqual(Real(1.0), m[0][0]);
      Assert::AreEqual(Real(4.22), m[0][1]);
      Assert::AreEqual(Real(7.89), m[0][2]);
      Assert::AreEqual(Real(2.2), m[1][0]);
      Assert::AreEqual(Real(5.1), m[1][1]);
      Assert::AreEqual(Real(8.32), m[1][2]);
      Assert::AreEqual(Real(3.0), m[2][0]);
      Assert::AreEqual(Real(6.7), m[2][1]);
      Assert::AreEqual(Real(9.21), m[2][2]);
    }

    TEST_METHOD(Matrix3GetCofactor)
    {
      Matrix3 m;
      m.Set(
        2.0, 4.0, 5.0,
        3.0, 6.0, 7.0,
        10.1, 8.32, 9.129
      );

      Matrix3 mc = m.GetCofactorMatrix();
      Assert::IsTrue(AreEqual(Real(-3.4659996), mc[0][0]));
      Assert::IsTrue(AreEqual(Real(43.313003), mc[0][1]));
      Assert::IsTrue(AreEqual(Real(-35.6400032), mc[0][2]));
      Assert::IsTrue(AreEqual(Real(5.08399963), mc[1][0]));
      Assert::IsTrue(AreEqual(Real(-32.2420006), mc[1][1]));
      Assert::IsTrue(AreEqual(Real(23.7600021), mc[1][2]));
      Assert::IsTrue(AreEqual(Real(-2.0), mc[2][0]));
      Assert::IsTrue(AreEqual(Real(1.0), mc[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), mc[2][2]));

      mc = m.GetAdjointMatrix();
      Assert::IsTrue(AreEqual(Real(-3.4659996), mc[0][0]));
      Assert::IsTrue(AreEqual(Real(5.08399963), mc[0][1]));
      Assert::IsTrue(AreEqual(Real(-2.0), mc[0][2]));
      Assert::IsTrue(AreEqual(Real(43.313003), mc[1][0]));
      Assert::IsTrue(AreEqual(Real(-32.2420006), mc[1][1]));
      Assert::IsTrue(AreEqual(Real(1.0), mc[1][2]));
      Assert::IsTrue(AreEqual(Real(-35.6400032), mc[2][0]));
      Assert::IsTrue(AreEqual(Real(23.7600021), mc[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), mc[2][2]));
    }

    TEST_METHOD(Matrix3Inverse)
    {
      Matrix3 m;
      m.Set(
        2.0, 4.0, 5.0,
        3.0, 6.0, 7.0,
        10.1, 8.32, 9.129
      );

      Matrix3 mi = m.Inverted();
      Assert::IsTrue(AreEqual(Real(0.291750699), mi[0][0]));
      Assert::IsTrue(AreEqual(Real(-0.427945912), mi[0][1]));
      Assert::IsTrue(AreEqual(Real(0.168350101), mi[0][2]));
      Assert::IsTrue(AreEqual(Real(-3.64587426), mi[1][0]));
      Assert::IsTrue(AreEqual(Real(2.71397209), mi[1][1]));
      Assert::IsTrue(AreEqual(Real(-0.0841750503), mi[1][2]));
      Assert::IsTrue(AreEqual(Real(2.99999905), mi[2][0]));
      Assert::IsTrue(AreEqual(Real(-1.9999994), mi[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), mi[2][2]));

      m.Inverse();
      Assert::IsTrue(AreEqual(Real(0.291750699), m[0][0]));
      Assert::IsTrue(AreEqual(Real(-0.427945912), m[0][1]));
      Assert::IsTrue(AreEqual(Real(0.168350101), m[0][2]));
      Assert::IsTrue(AreEqual(Real(-3.64587426), m[1][0]));
      Assert::IsTrue(AreEqual(Real(2.71397209), m[1][1]));
      Assert::IsTrue(AreEqual(Real(-0.0841750503), m[1][2]));
      Assert::IsTrue(AreEqual(Real(2.99999905), m[2][0]));
      Assert::IsTrue(AreEqual(Real(-1.9999994), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));
    }

    TEST_METHOD(Matrix3OpAdd)
    {
      Matrix3 m1;
      Matrix3 m2;

      m1.Set(
        1.0, 4.0, -5.2,
        0.4, -4.5, 2.0,
        3.3, 10.1, 5.0
      );
      m2.Set(
        0.0, -3.0, 5.0,
        0.6, 1.5, 3.9,
        -0.3, -10.0, 5.0
      );

      Matrix3 mr = m1 + m2;
      Assert::IsTrue(AreEqual(Real(1.0), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(1.0), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(-0.1999998), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(1.0), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-3.0), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(5.9000001), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(3.0), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(0.100000381), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(10.0), mr[2][2]));

      m1 += m2;
      Assert::IsTrue(AreEqual(Real(1.0), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(-0.1999998), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(-3.0), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(5.9000001), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(3.0), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(0.100000381), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(10.0), m1[2][2]));
    }

    TEST_METHOD(Matrix3OpSub)
    {
      Matrix3 m1;
      Matrix3 m2;

      m1.Set(
        2.5, -2.5, 10.2,
        4.4, 12.4, -9.3,
        10.1, -4.2, 11.9
      );
      m2.Set(
        2.0, -4.5, 8.1,
        6.2, 10.0, -8.0,
        8.0, -5.6, 10.9
      );

      Matrix3 mr = m1 - m2;
      Assert::IsTrue(AreEqual(Real(0.5), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(2.0), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(2.09999943), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-1.79999971), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(2.39999962), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(-1.30000019), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(2.10000038), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(1.40000010), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(1.0), mr[2][2]));

      m1 -= m2;
      Assert::IsTrue(AreEqual(Real(0.5), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(2.0), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(2.09999943), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-1.79999971), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(2.39999962), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(-1.30000019), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(2.10000038), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(1.40000010), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(1.0), m1[2][2]));
    }

    TEST_METHOD(Matrix3Mul)
    {
      Matrix3 m1;
      Matrix3 m2;

      m1.Set(
        2.5, -2.5, 10.2,
        4.4, 12.4, -9.3,
        10.1, -4.2, 11.9
      );
      m2.Set(
        2.0, -4.5, 8.1,
        6.2, 10.0, -8.0,
        8.0, -5.6, 10.9
      );

      Matrix3 mr = m1 * m2;
      Assert::IsTrue(AreEqual(Real(71.0999985), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-93.3699951), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(151.429993), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(11.2799959), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(156.279999), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(-164.929993), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(89.3600006), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-154.089996), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(245.119995), mr[2][2]));

      m1 *= m2;

      Assert::IsTrue(AreEqual(Real(71.0999985), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(-93.3699951), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(151.429993), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(11.2799959), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(156.279999), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(-164.929993), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(89.3600006), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(-154.089996), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(245.119995), m1[2][2]));

      m1.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);
      m2.Set(
        5.0, -10.2, 22.0,
        11.0, 0.0, 4.3,
        16.16, -15.0, 20.5
      );

      mr = m1 * m2;
      Assert::IsTrue(AreEqual(Real(119.791199), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-78.7800064), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(120.276009), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(-62.6191177), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-83.0399933), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(81.0809937), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(1220.79321), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-1164.85864), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(1663.32080), mr[2][2]));

      m1 *= m2;

      Assert::IsTrue(AreEqual(Real(119.791199), m1[0][0]));
      Assert::IsTrue(AreEqual(Real(-78.7800064), m1[0][1]));
      Assert::IsTrue(AreEqual(Real(120.276009), m1[0][2]));
      Assert::IsTrue(AreEqual(Real(-62.6191177), m1[1][0]));
      Assert::IsTrue(AreEqual(Real(-83.0399933), m1[1][1]));
      Assert::IsTrue(AreEqual(Real(81.0809937), m1[1][2]));
      Assert::IsTrue(AreEqual(Real(1220.79321), m1[2][0]));
      Assert::IsTrue(AreEqual(Real(-1164.85864), m1[2][1]));
      Assert::IsTrue(AreEqual(Real(1663.32080), m1[2][2]));
    }

    TEST_METHOD(Matrix3MulScalar)
    {
      Matrix3 m1, m2, mr;

      m1.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);
      m2.Set(
        5.0, -10.2, 22.0,
        11.0, 0.0, 4.3,
        16.16, -15.0, 20.5
      );

      mr = m1 * 1.5;
      Assert::IsTrue(AreEqual(Real(-0.150000006), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(4.67999983), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(7.98000050), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(3.89999986), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(-18.6149998), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(5.65199995), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(12.2144995), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(1.99349999), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(108.180008), mr[2][2]));

      mr = m2 * 0.4;

      Assert::IsTrue(AreEqual(Real(2.0), mr[0][0]));
      Assert::IsTrue(AreEqual(Real(-4.07999992), mr[0][1]));
      Assert::IsTrue(AreEqual(Real(8.80000019), mr[0][2]));
      Assert::IsTrue(AreEqual(Real(4.40000010), mr[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), mr[1][1]));
      Assert::IsTrue(AreEqual(Real(1.72000015), mr[1][2]));
      Assert::IsTrue(AreEqual(Real(6.46400023), mr[2][0]));
      Assert::IsTrue(AreEqual(Real(-6.0), mr[2][1]));
      Assert::IsTrue(AreEqual(Real(8.19999981), mr[2][2]));
    }

    TEST_METHOD(Matrix3MulVector3)
    {
      Matrix3 m1, m2;
      Vector3 v, vr;

      m1.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);
      m2.Set(
        5.0, -10.2, 22.0,
        11.0, 0.0, 4.3,
        16.16, -15.0, 20.5
      );
      v.Set(
        2.4, -3.89, 14.8
      );

      vr = m1 *  v;
      Assert::IsTrue(AreEqual(Real(66.3591995), vr[0]));
      Assert::IsTrue(AreEqual(Real(110.281303), vr[1]));
      Assert::IsTrue(AreEqual(Real(1081.74951), vr[2]));

      vr = m2 *  v;
      Assert::IsTrue(AreEqual(Real(377.278015), vr[0]));
      Assert::IsTrue(AreEqual(Real(90.0400085), vr[1]));
      Assert::IsTrue(AreEqual(Real(400.533997), vr[2]));
    }

    TEST_METHOD(Matrix3OpEq)
    {
      Matrix3 m1, m2;
      m1.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);
      m2.Set(
        5.0, -10.2, 22.0,
        11.0, 0.0, 4.3,
        16.16, -15.0, 20.5
      );

      Assert::IsTrue(m1 != m2);
      Assert::IsFalse(m1 == m2);

      m1.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);
      m2.Set(
        -0.1, 3.12, 5.32,
        2.6, -12.41, 3.768,
        8.143, 1.329, 72.12);

      Assert::IsTrue(m1 == m2);
      Assert::IsFalse(m1 != m2);
    }
  };
}

#pragma warning(default : 4305)