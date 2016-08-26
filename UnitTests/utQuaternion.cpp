#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/Engine/Math/sfMathDefs.hpp"
#include "../src/Engine/Math/sfMatrix3.hpp"
#include "../src/Engine/Math/sfMatrix4.hpp"
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
    TEST_METHOD(QuatCtorAxisAngle)
    {
      Quaternion q1(Vector3::c_xAxis, 0), q2(Vector3::c_yAxis, SF_PI / 2);

      Assert::IsTrue(AreEqual(Real(1.0), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));

      Assert::IsTrue(AreEqual(Real(0.707106769), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.707106769), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[3]));

      q1 = Quaternion(Vector3::c_xAxis, 105 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(0.608761430), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.793353319), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));

      q2 = Quaternion(Vector3::c_zAxis, 89 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(0.713250458), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.700909257), q2[3]));

      q1 = Quaternion(Vector3::c_yAxis, 240 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(-0.500000060), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.866025388), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));

    }

    TEST_METHOD(QuatCtorReal)
    {
      Quaternion q = Quaternion(1.0, -0.5, 0.5, 0.1);

      Assert::IsTrue(AreEqual(Real(1.0), q[0]));
      Assert::IsTrue(AreEqual(Real(-0.5), q[1]));
      Assert::IsTrue(AreEqual(Real(0.5), q[2]));
      Assert::IsTrue(AreEqual(Real(0.1), q[3]));
    }

    TEST_METHOD(QuatCtorMatrix3)
    {
      Matrix3 m;
      m.Set(Real(0.0), Real(0.0), Real(1.0),
        Real(0.0), Real(1.0), Real(0.0),
        Real(-1.0), Real(0.0), Real(0.0));

      Quaternion q(m);
      Assert::IsTrue(AreEqual(Real(0.707106769), q[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q[1]));
      Assert::IsTrue(AreEqual(Real(0.707106769), q[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q[3]));
    }

    TEST_METHOD(QuatCtorMatrix4)
    {
      Matrix4 m;
      m.Set(Real(0.0), Real(0.0), Real(1.0), Real(0.0),
        Real(0.0), Real(1.0), Real(0.0), Real(0.0),
        Real(-1.0), Real(0.0), Real(0.0), Real(0.0),
        Real(0.0), Real(0.0), Real(0.0), Real(1.0));

      Quaternion q(m);
      Assert::IsTrue(AreEqual(Real(0.707106769), q[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q[1]));
      Assert::IsTrue(AreEqual(Real(0.707106769), q[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q[3]));
    }

    TEST_METHOD(QuatGetMatrix3)
    {
      Quaternion q1(0.5, 0.5, 0.5, 0.5), q2(0.0, 0.707106769, 0.0, -0.707106769);

      Matrix3 m = q1.GetMatrix3();
      Assert::IsTrue(AreEqual(Real(0.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(1.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(1.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));

      m = q2.GetMatrix3();
      Assert::IsTrue(AreEqual(Real(0.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));
    }

    TEST_METHOD(QuatGetMatrix4)
    {
      Quaternion q1(0.5, 0.5, 0.5, 0.5), q2(0.0, 0.707106769, 0.0, -0.707106769);

      Matrix4 m = q1.GetMatrix4();
      Assert::IsTrue(AreEqual(Real(0.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(1.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][3]));
      Assert::IsTrue(AreEqual(Real(1.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(1.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m[3][3]));

      m = q2.GetMatrix4();
      Assert::IsTrue(AreEqual(Real(0.0), m[0][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][1]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[0][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[0][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][0]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[1][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[1][3]));
      Assert::IsTrue(AreEqual(Real(-1.0), m[2][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][2]));
      Assert::IsTrue(AreEqual(Real(0.0), m[2][3]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][0]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][1]));
      Assert::IsTrue(AreEqual(Real(0.0), m[3][2]));
      Assert::IsTrue(AreEqual(Real(1.0), m[3][3]));
    }

    TEST_METHOD(QuatSetAxisAngle)
    {
      Quaternion q1, q2;

      q1.Set(Vector3::c_xAxis, 0);
      q2.Set(Vector3::c_yAxis, SF_PI / 2);

      Assert::IsTrue(AreEqual(Real(1.0), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));

      Assert::IsTrue(AreEqual(Real(0.707106769), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.707106769), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[3]));

      q1.Set(Vector3::c_xAxis, 105 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(0.608761430), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.793353319), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));

      q2.Set(Vector3::c_zAxis, 89 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(0.713250458), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.0), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.700909257), q2[3]));

      q1.Set(Vector3::c_yAxis, 240 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(-0.500000060), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.866025388), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.0), q1[3]));
    }

    TEST_METHOD(QuatSetEuler)
    {
      Quaternion q;
      q.SetEuler(30 * SF_RADS_PER_DEG, 0.0, 45 * SF_RADS_PER_DEG);

      Assert::IsTrue(AreEqual(Real(0.8923991008325228), q[0]));
      Assert::IsTrue(AreEqual(Real(0.23911761839433449), q[1]));
      Assert::IsTrue(AreEqual(Real(0.3696438106143861), q[2]));
      Assert::IsTrue(AreEqual(Real(-0.09904576054128762), q[3]));

      q.SetEuler(30 * SF_RADS_PER_DEG, 67 * SF_RADS_PER_DEG,
        -25 * SF_RADS_PER_DEG);
      Assert::IsTrue(AreEqual(Real(0.817297741871787), q[0]));
      Assert::IsTrue(AreEqual(Real(0.09531911311484745), q[1]));
      Assert::IsTrue(AreEqual(Real(-0.0348703712600556), q[2]));
      Assert::IsTrue(AreEqual(Real(0.5672060692680064), q[3]));
    }

    TEST_METHOD(QuatGetEulerXYZ)
    {
      Real roll, pitch, yaw;
      Quaternion q;

      q.Set(Real(0.8923991008325228), Real(0.23911761839433449),
        Real(0.3696438106143861), Real(-0.09904576054128762));

      q.GetEulerXYZ(roll, pitch, yaw);
      Assert::IsTrue(AreEqual(30 * SF_RADS_PER_DEG, roll));
      Assert::IsTrue(AreEqual(Real(0.0), pitch));
      Assert::IsTrue(AreEqual(45 * SF_RADS_PER_DEG, yaw));

      q.Set(Real(0.817297741871787), Real(0.09531911311484745),
        Real(-0.0348703712600556), Real(0.5672060692680064));
      q.GetEulerXYZ(roll, pitch, yaw);
      Assert::IsTrue(AreEqual(30 * SF_RADS_PER_DEG, roll));
      Assert::IsTrue(AreEqual(67 * SF_RADS_PER_DEG, pitch));
      Assert::IsTrue(AreEqual(-25 * SF_RADS_PER_DEG, yaw));
    }

    TEST_METHOD(QuatSetReal)
    {
      Quaternion q;
      q.Set(1.0, -0.5, 0.5, 0.1);

      Assert::IsTrue(AreEqual(Real(1.0), q[0]));
      Assert::IsTrue(AreEqual(Real(-0.5), q[1]));
      Assert::IsTrue(AreEqual(Real(0.5), q[2]));
      Assert::IsTrue(AreEqual(Real(0.1), q[3]));
    }

    TEST_METHOD(QuatGetAxisAngle)
    {
      Quaternion q;
      Vector3 axis;
      Real angle;

      q.Set(Real(1.0), Real(0.0), Real(0.0), Real(0.0));
      q.GetAxisAngle(axis, angle);

      Assert::IsTrue(axis == Vector3::c_xAxis);
      Assert::IsTrue(AreEqual(Real(0.0), angle));

      q.Set(Real(0.707106769), Real(0.0), Real(0.707106769), Real(0.0));
      q.GetAxisAngle(axis, angle);

      Assert::IsTrue(std::abs(axis[0] - Vector3::c_yAxis[0]) < 0.001);
      Assert::IsTrue(std::abs(axis[1] - Vector3::c_yAxis[1]) < 0.001);
      Assert::IsTrue(std::abs(axis[2] - Vector3::c_yAxis[2]) < 0.001);
      Assert::IsTrue(AreEqual(Real(SF_PI / 2), angle));

      q.Set(Real(0.608761430), Real(0.793353319), Real(0.0), Real(0.0));
      q.GetAxisAngle(axis, angle);

      Assert::IsTrue(std::abs(axis[0] - Vector3::c_xAxis[0]) < 0.001);
      Assert::IsTrue(std::abs(axis[1] - Vector3::c_xAxis[1]) < 0.001);
      Assert::IsTrue(std::abs(axis[2] - Vector3::c_xAxis[2]) < 0.001);
      Assert::IsTrue(AreEqual(Real(105 * SF_RADS_PER_DEG), angle));

      q.Set(Real(0.713250458), Real(0.0), Real(0.0), Real(0.700909257));
      q.GetAxisAngle(axis, angle);

      Assert::IsTrue(std::abs(axis[0] - Vector3::c_zAxis[0]) < 0.001);
      Assert::IsTrue(std::abs(axis[1] - Vector3::c_zAxis[1]) < 0.001);
      Assert::IsTrue(std::abs(axis[2] - Vector3::c_zAxis[2]) < 0.001);
      Assert::IsTrue(AreEqual(Real(89 * SF_RADS_PER_DEG), angle));

      q.Set(Real(-0.500000060), Real(0.0), Real(0.866025388), Real(0.0));
      q.GetAxisAngle(axis, angle);

      Assert::IsTrue(std::abs(axis[0] - Vector3::c_yAxis[0]) < 0.001);
      Assert::IsTrue(std::abs(axis[1] - Vector3::c_yAxis[1]) < 0.001);
      Assert::IsTrue(std::abs(axis[2] - Vector3::c_yAxis[2]) < 0.001);
      Assert::IsTrue(AreEqual(Real(240 * SF_RADS_PER_DEG), angle));
    }

    TEST_METHOD(QuatInv)
    {
      Quaternion q(1.0, 0.5, 0.4, -1.0), qi;

      qi = q.Inverted();
      Assert::IsTrue(AreEqual(Real(1.0), qi[0]));
      Assert::IsTrue(AreEqual(Real(-0.5), qi[1]));
      Assert::IsTrue(AreEqual(Real(-0.4), qi[2]));
      Assert::IsTrue(AreEqual(Real(1.0), qi[3]));

      q.Invert();
      Assert::IsTrue(AreEqual(Real(1.0), q[0]));
      Assert::IsTrue(AreEqual(Real(-0.5), q[1]));
      Assert::IsTrue(AreEqual(Real(-0.4), q[2]));
      Assert::IsTrue(AreEqual(Real(1.0), q[3]));
    }

    TEST_METHOD(QuatLength)
    {
      Quaternion q1(0.7, -0.4, 1.0, 5.0), q2(-2.3, 3.3, 2.0, 2.7);

      Assert::IsTrue(AreEqual(Real(5.16236379966), q1.Length()));
      Assert::IsTrue(AreEqual(Real(5.24118307255), q2.Length()));

      Assert::IsTrue(AreEqual(Real(26.6499996), q1.LengthSq()));
      Assert::IsTrue(AreEqual(Real(27.4700012), q2.LengthSq()));
    }

    TEST_METHOD(QuatNorm)
    {
      Quaternion q1(0.7, -0.4, 1.0, 5.0), q2(-2.3, 3.3, 2.0, 2.7);
      Quaternion q1n, q2n;
      q1n = q1.Normalized();
      q2n = q2.Normalized();

      Assert::IsTrue(AreEqual(Real(0.135586545), q1n[0]));
      Assert::IsTrue(AreEqual(Real(-0.0774780288), q1n[1]));
      Assert::IsTrue(AreEqual(Real(0.193695068), q1n[2]));
      Assert::IsTrue(AreEqual(Real(0.968475342), q1n[3]));

      Assert::IsTrue(AreEqual(Real(-0.438830554), q2n[0]));
      Assert::IsTrue(AreEqual(Real(0.629626453), q2n[1]));
      Assert::IsTrue(AreEqual(Real(0.381591797), q2n[2]));
      Assert::IsTrue(AreEqual(Real(0.515148938), q2n[3]));

      q1.Normalize();
      q2.Normalize();
      Assert::IsTrue(AreEqual(Real(0.135586545), q1[0]));
      Assert::IsTrue(AreEqual(Real(-0.0774780288), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.193695068), q1[2]));
      Assert::IsTrue(AreEqual(Real(0.968475342), q1[3]));

      Assert::IsTrue(AreEqual(Real(-0.438830554), q2[0]));
      Assert::IsTrue(AreEqual(Real(0.629626453), q2[1]));
      Assert::IsTrue(AreEqual(Real(0.381591797), q2[2]));
      Assert::IsTrue(AreEqual(Real(0.515148938), q2[3]));
    }

    TEST_METHOD(QuatAddOp)
    {
      Quaternion q1(-1.2, 2.3, 5.0, -4.7), q2(1.2, 2.0, -4.7, 4.0);

      Quaternion res = q1 + q2;
      Assert::IsTrue(AreEqual(Real(0.0), res[0]));
      Assert::IsTrue(AreEqual(Real(4.3), res[1]));
      Assert::IsTrue(AreEqual(Real(0.3), res[2]));
      Assert::IsTrue(AreEqual(Real(-0.7), res[3]));

      q1 += q2;
      Assert::IsTrue(AreEqual(Real(0.0), q1[0]));
      Assert::IsTrue(AreEqual(Real(4.3), q1[1]));
      Assert::IsTrue(AreEqual(Real(0.3), q1[2]));
      Assert::IsTrue(AreEqual(Real(-0.7), q1[3]));
    }

    TEST_METHOD(QuatSubOp)
    {
      Quaternion q1(-1.2, 2.3, 5.0, -4.7), q2(1.2, 2.0, -4.7, 4.0);

      Quaternion res = q1 - q2;
      Assert::IsTrue(AreEqual(Real(-2.4), res[0]));
      Assert::IsTrue(AreEqual(Real(0.3), res[1]));
      Assert::IsTrue(AreEqual(Real(9.7), res[2]));
      Assert::IsTrue(AreEqual(Real(-8.7), res[3]));

      q1 -= q2;
      Assert::IsTrue(AreEqual(Real(-2.4), q1[0]));
      Assert::IsTrue(AreEqual(Real(0.3), q1[1]));
      Assert::IsTrue(AreEqual(Real(9.7), q1[2]));
      Assert::IsTrue(AreEqual(Real(-8.7), q1[3]));
    }

    TEST_METHOD(QuarScalarMul)
    {
      Quaternion q(1.0, 2.0, -2.2, 5.6);
      Real s = 0.5;

      Quaternion res = q * s;

      Assert::IsTrue(AreEqual(Real(0.5), res[0]));
      Assert::IsTrue(AreEqual(Real(1.0), res[1]));
      Assert::IsTrue(AreEqual(Real(-1.10000002), res[2]));
      Assert::IsTrue(AreEqual(Real(2.79999995), res[3]));

      q *= s;
      Assert::IsTrue(AreEqual(Real(0.5), res[0]));
      Assert::IsTrue(AreEqual(Real(1.0), res[1]));
      Assert::IsTrue(AreEqual(Real(-1.10000002), res[2]));
      Assert::IsTrue(AreEqual(Real(2.79999995), res[3]));
    }

    TEST_METHOD(QuarScalarDiv)
    {
      Quaternion q(1.0, 2.0, -2.2, 5.6);
      Real s = 2.0;

      Quaternion res = q / s;

      Assert::IsTrue(AreEqual(Real(0.5), res[0]));
      Assert::IsTrue(AreEqual(Real(1.0), res[1]));
      Assert::IsTrue(AreEqual(Real(-1.10000002), res[2]));
      Assert::IsTrue(AreEqual(Real(2.79999995), res[3]));

      q /= s;
      Assert::IsTrue(AreEqual(Real(0.5), res[0]));
      Assert::IsTrue(AreEqual(Real(1.0), res[1]));
      Assert::IsTrue(AreEqual(Real(-1.10000002), res[2]));
      Assert::IsTrue(AreEqual(Real(2.79999995), res[3]));
    }

    TEST_METHOD(QuatMulQuat)
    {
      Quaternion q1(0.5, 1.1, -0.32, 0.64), q2(-0.2, 0.4, 1.2, 3.1);
      Quaternion q3(-0.13, -2.1, 10.1, 4.1), q4(1.3, 3.3, -9.1, -10.2);
      
      Quaternion res1 = q1 * q2;
      Assert::IsTrue(AreEqual(Real(-2.13999987), res1[0]));
      Assert::IsTrue(AreEqual(Real(-1.77999997), res1[1]));
      Assert::IsTrue(AreEqual(Real(-2.49000001), res1[2]));
      Assert::IsTrue(AreEqual(Real(2.86999989), res1[3]));

      Quaternion res2 = q3 * q4;
      Assert::IsTrue(AreEqual(Real(140.490997), res2[0]));
      Assert::IsTrue(AreEqual(Real(-68.8690033), res2[1]));
      Assert::IsTrue(AreEqual(Real(6.42300034), res2[2]));
      Assert::IsTrue(AreEqual(Real(-7.56400108), res2[3]));

      q1 *= q2;
      Assert::IsTrue(AreEqual(Real(-2.13999987), q1[0]));
      Assert::IsTrue(AreEqual(Real(-1.77999997), q1[1]));
      Assert::IsTrue(AreEqual(Real(-2.49000001), q1[2]));
      Assert::IsTrue(AreEqual(Real(2.86999989), q1[3]));

      q3 *= q4;
      Assert::IsTrue(AreEqual(Real(140.490997), q3[0]));
      Assert::IsTrue(AreEqual(Real(-68.8690033), q3[1]));
      Assert::IsTrue(AreEqual(Real(6.42300034), q3[2]));
      Assert::IsTrue(AreEqual(Real(-7.56400108), q3[3]));
    }

    TEST_METHOD(QuatMulVec)
    {
      //Multiplying quaternion and vector is the same as multiplying
      //quaternion and pure quaternion, where vector part is constructed
      //from the original vector

      Quaternion q1(0.5, 1.1, -0.32, 0.64), q2(0.0, 0.4, 1.2, 3.1);
      Quaternion q3(-0.13, -2.1, 10.1, 4.1), q4(0.0, 3.3, -9.1, -10.2);
      Vector3 v1(0.4, 1.2, 3.1), v2(3.3, -9.1, -10.2);

      Quaternion resqq1 = q1 * q2;
      Quaternion resqv1 = q1 * v1;

      Assert::IsTrue(AreEqual(resqq1[0], resqv1[0]));
      Assert::IsTrue(AreEqual(resqq1[1], resqv1[1]));
      Assert::IsTrue(AreEqual(resqq1[2], resqv1[2]));
      Assert::IsTrue(AreEqual(resqq1[3], resqv1[3]));

      Quaternion resqq2 = q3 * q4;
      Quaternion resqv2 = q3 * v2;

      Assert::IsTrue(AreEqual(resqq2[0], resqv2[0]));
      Assert::IsTrue(AreEqual(resqq2[1], resqv2[1]));
      Assert::IsTrue(AreEqual(resqq2[2], resqv2[2]));
      Assert::IsTrue(AreEqual(resqq2[3], resqv2[3]));

      //Change order
      resqq1 = q2 * q1;
      resqv1 = v1 * q1;

      Assert::IsTrue(AreEqual(resqq1[0], resqv1[0]));
      Assert::IsTrue(AreEqual(resqq1[1], resqv1[1]));
      Assert::IsTrue(AreEqual(resqq1[2], resqv1[2]));
      Assert::IsTrue(AreEqual(resqq1[3], resqv1[3]));

      resqq2 = q4 * q3;
      resqv2 = v2 * q3;

      Assert::IsTrue(AreEqual(resqq2[0], resqv2[0]));
      Assert::IsTrue(AreEqual(resqq2[1], resqv2[1]));
      Assert::IsTrue(AreEqual(resqq2[2], resqv2[2]));
      Assert::IsTrue(AreEqual(resqq2[3], resqv2[3]));
    }

    TEST_METHOD(QuatSlerp)
    {
      Quaternion q1(0.5, 1.1, -0.32, 0.64), q2(-0.2, 0.4, 1.2, 3.1);
      Quaternion q3(-0.13, -2.1, 10.1, 4.1), q4(1.3, 3.3, -9.1, -10.2);

      q1.Normalize();
      q2.Normalize();
      q3.Normalize();
      q4.Normalize();

      Quaternion res1 = q1.Slerp(q2, 0.75);

      Assert::IsTrue(AreEqual(Real(0.0608885102), res1[0]));
      Assert::IsTrue(AreEqual(Real(0.342188656), res1[1]));
      Assert::IsTrue(AreEqual(Real(0.226777509), res1[2]));
      Assert::IsTrue(AreEqual(Real(0.909881353), res1[3]));

      Quaternion res2 = q3.Slerp(q4, 0.5);

      Assert::IsTrue(AreEqual(Real(0.177813023), res2[0]));
      Assert::IsTrue(AreEqual(Real(0.0985467434), res2[1]));
      Assert::IsTrue(AreEqual(Real(0.587207198), res2[2]));
      Assert::IsTrue(AreEqual(Real(-0.781193376), res2[3]));

      res1 = q1.Slerp(q2, 1.0);
      Assert::IsTrue(AreEqual(q2[0], res1[0]));
      Assert::IsTrue(AreEqual(q2[1], res1[1]));
      Assert::IsTrue(AreEqual(q2[2], res1[2]));
      Assert::IsTrue(AreEqual(q2[3], res1[3]));

      res1 = q1.Slerp(q2, 0.0);
      Assert::IsTrue(AreEqual(q1[0], res1[0]));
      Assert::IsTrue(AreEqual(q1[1], res1[1]));
      Assert::IsTrue(AreEqual(q1[2], res1[2]));
      Assert::IsTrue(AreEqual(q1[3], res1[3]));
    }

    TEST_METHOD(QuatRotateVec)
    {
      Vector3 v1(1, 0, 0);
      Quaternion q1(0.7071, 0.0, 0.0, 0.7071);

      Vector3 res = q1.Rotated(v1);
      Assert::IsTrue(AreEqual(Real(0.0), res[0]));
      Assert::IsTrue(AreEqual(Real(0.999980748), res[1]));
      Assert::IsTrue(AreEqual(Real(0.0), res[2]));

      q1.Rotate(v1);
      Assert::IsTrue(AreEqual(Real(0.0), v1[0]));
      Assert::IsTrue(AreEqual(Real(0.999980748), v1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), v1[2]));

      v1.Set(0, 1.0, 0.0);
      q1.Set(Vector3::c_zAxis, -SF_PI / 2);
      q1.Rotate(v1);
      Assert::IsTrue(AreEqual(Real(0.999999940), v1[0]));
      Assert::IsTrue(AreEqual(Real(0.0), v1[1]));
      Assert::IsTrue(AreEqual(Real(0.0), v1[2]));
    }
  };
}