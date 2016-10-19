#include "stdafx.h"
#include "CppUnitTest.h"
#include "Math/sfMathDefs.hpp"
#include "Math/sfVector4.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

namespace UnitTests
{
  template<typename T>
  static bool AreEqual(const T &expected, const T &actual)
  {
    if (std::abs(expected - actual) < SF_UT_EPSILON)
      return true;

    return false;
  }

  TEST_CLASS(utVector3)
  {
  public:
    TEST_METHOD(Vector4Static)
    {
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zero.GetX()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zero.GetY()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zero.GetZ()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zero.GetW()));

      Assert::IsTrue(AreEqual(Real(1.0), Vector4::c_xAxis.GetX()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_xAxis.GetY()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_xAxis.GetZ()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_xAxis.GetW()));

      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_yAxis.GetX()));
      Assert::IsTrue(AreEqual(Real(1.0), Vector4::c_yAxis.GetY()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_yAxis.GetZ()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_yAxis.GetW()));

      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zAxis.GetX()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zAxis.GetY()));
      Assert::IsTrue(AreEqual(Real(1.0), Vector4::c_zAxis.GetZ()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_zAxis.GetW()));

      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_wAxis.GetX()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_wAxis.GetY()));
      Assert::IsTrue(AreEqual(Real(0.0), Vector4::c_wAxis.GetZ()));
      Assert::IsTrue(AreEqual(Real(1.0), Vector4::c_wAxis.GetW()));
    }

    TEST_METHOD(Vector4Ctor)
    {
      Vector4 v(Real(0.123), Real(-12.32), Real(32.2), Real(55.1234));

      Assert::IsTrue(AreEqual(Real(0.123), v.GetX()));
      Assert::IsTrue(AreEqual(Real(-12.32), v.GetY()));
      Assert::IsTrue(AreEqual(Real(32.2), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(55.1234), v.GetW()));

#ifdef SF_USE_SIMD
      Vector4 vv(_mm_set_ps(-5.232f, 123.332f, 0.001f, -23.4543f));

      Assert::IsTrue(AreEqual(Real(-23.4543), vv.GetX()));
      Assert::IsTrue(AreEqual(Real(0.001), vv.GetY()));
      Assert::IsTrue(AreEqual(Real(123.332), vv.GetZ()));
      Assert::IsTrue(AreEqual(Real(-5.232), vv.GetW()));
#endif
    }

    TEST_METHOD(Vector4AssignOp)
    {
      Vector4 v(Real(0.123), Real(-12.32), Real(32.2), Real(55.1234));
      Vector4 vv(Real(-5.232), Real(123.332), Real(0.001), Real(-23.4543));

      v = vv;

      Assert::IsTrue(AreEqual(Real(-5.232), v.GetX()));
      Assert::IsTrue(AreEqual(Real(123.332), v.GetY()));
      Assert::IsTrue(AreEqual(Real(0.001), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(-23.4543), v.GetW()));
    }

    TEST_METHOD(Vector4SetGet)
    {
      Vector4 v;

      v.Set(Real(0.123), Real(-12.32), Real(32.2), Real(55.1234));
      Assert::IsTrue(AreEqual(Real(0.123), v.GetX()));
      Assert::IsTrue(AreEqual(Real(-12.32), v.GetY()));
      Assert::IsTrue(AreEqual(Real(32.2), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(55.1234), v.GetW()));
      
      v.SetX(Real(12321.213));
      Assert::IsTrue(AreEqual(Real(12321.213), v.GetX()));

      v.SetY(Real(121.5567));
      Assert::IsTrue(AreEqual(Real(121.5567), v.GetY()));

      v.SetZ(Real(-15.0023));
      Assert::IsTrue(AreEqual(Real(-15.0023), v.GetZ()));

      v.SetW(Real(0.0012));
      Assert::IsTrue(AreEqual(Real(0.0012), v.GetW()));

#ifdef SF_USE_SIMD
      v.Set(_mm_set_ps(-5.232f, 123.332f, 0.001f, -23.4543f));

      Assert::IsTrue(AreEqual(Real(-23.4543), v.GetX()));
      Assert::IsTrue(AreEqual(Real(0.001), v.GetY()));
      Assert::IsTrue(AreEqual(Real(123.332), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(-5.232), v.GetW()));
#endif
    }

    TEST_METHOD(Vector4ZeroOut)
    {
      Vector4 v(Real(0.123), Real(-12.32), Real(32.2), Real(55.1234));

      v.ZeroOut();
      Assert::IsTrue(AreEqual(Real(0.0), v.GetX()));
      Assert::IsTrue(AreEqual(Real(0.0), v.GetY()));
      Assert::IsTrue(AreEqual(Real(0.0), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(0.0), v.GetW()));
    }

    TEST_METHOD(Vector4Dot)
    {
      Real d;
      Vector4 v1(Real(-0.231), Real(142.23), Real(-234.3), Real(222.1));
      Vector4 v2(Real(5.4522), Real(10.232), Real(102.3), Real(23.1));

      d = v1.Dot(v2);
      Assert::IsTrue(AreEqual(Real(-17384.3438), d));

      d = Dot(v1, v2);
      Assert::IsTrue(AreEqual(Real(-17384.3438), d));

      v1.Set(Real(0.2), Real(1.3), Real(3.21), Real(-2.3));
      v2.Set(Real(-1.5), Real(-5.2), Real(4.1), Real(-9.9));

      d = v1.Dot(v2);
      Assert::IsTrue(AreEqual(Real(28.871002), d));

      d = Dot(v1, v2);
      Assert::IsTrue(AreEqual(Real(28.871002), d));

      Vector3 v3(Real(1.2), Real(-5.4), Real(3.0));

      d = v1.Dot(v3);
      Assert::IsTrue(AreEqual(Real(0.55), d));

      d = Dot(v1, v3);
      Assert::IsTrue(AreEqual(Real(0.55), d));
    }

    TEST_METHOD(Vector4Length)
    {
      Vector4 v1(Real(-0.231), Real(142.23), Real(-234.3), Real(222.1));
      Vector4 v2(Real(5.4522), Real(10.232), Real(102.3), Real(23.1));

      Real l = v1.Length();
      Assert::IsTrue(AreEqual(Real(352.7808453), l));

      l = Length(v1);
      Assert::IsTrue(AreEqual(Real(352.7808453), l));

      l = v2.Length();
      Assert::IsTrue(AreEqual(Real(105.514557), l));

      l = Length(v2);
      Assert::IsTrue(AreEqual(Real(105.514557), l));

      l = v1.LengthSq();
      Assert::IsTrue(AreEqual(Real(124454.326261), l));

      l = LengthSq(v1);
      Assert::IsTrue(AreEqual(Real(124454.326261), l));

      l = v2.LengthSq();
      Assert::IsTrue(AreEqual(Real(11133.3213), l));

      l = LengthSq(v2);
      Assert::IsTrue(AreEqual(Real(11133.3213), l));
    }

    TEST_METHOD(Vector4Normalize)
    {
      Vector4 v1(Real(-0.231), Real(142.23), Real(-234.3), Real(222.1));
      Vector4 v2(Real(5.4522), Real(10.232), Real(102.3), Real(23.1));
      Vector4 r;

      r = v1.Normalized();
      Assert::IsTrue(AreEqual(Real(-0.0006547974), r[0]));
      Assert::IsTrue(AreEqual(Real(0.403168112), r[1]));
      Assert::IsTrue(AreEqual(Real(-0.664151728), r[2]));
      Assert::IsTrue(AreEqual(Real(0.629569352), r[3]));

      r.Set(Real(0.0), Real(0.0), Real(0.0), Real(0.0));
      r = Normalized(v1);
      Assert::IsTrue(AreEqual(Real(-0.0006547974), r[0]));
      Assert::IsTrue(AreEqual(Real(0.403168112), r[1]));
      Assert::IsTrue(AreEqual(Real(-0.664151728), r[2]));
      Assert::IsTrue(AreEqual(Real(0.629569352), r[3]));

      r = v2.Normalized();
      Assert::IsTrue(AreEqual(Real(0.051672494248), r[0]));
      Assert::IsTrue(AreEqual(Real(0.096972407865), r[1]));
      Assert::IsTrue(AreEqual(Real(0.969534531337), r[2]));
      Assert::IsTrue(AreEqual(Real(0.218927152237), r[3]));

      r.Set(Real(0.0), Real(0.0), Real(0.0), Real(0.0));
      r = Normalized(v2);
      Assert::IsTrue(AreEqual(Real(0.051672494248), r[0]));
      Assert::IsTrue(AreEqual(Real(0.096972407865), r[1]));
      Assert::IsTrue(AreEqual(Real(0.969534531337), r[2]));
      Assert::IsTrue(AreEqual(Real(0.218927152237), r[3]));

      v1.Normalize();
      Assert::IsTrue(AreEqual(Real(-0.0006547974), v1[0]));
      Assert::IsTrue(AreEqual(Real(0.403168112), v1[1]));
      Assert::IsTrue(AreEqual(Real(-0.664151728), v1[2]));
      Assert::IsTrue(AreEqual(Real(0.629569352), v1[3]));

      v2.Normalize();
      Assert::IsTrue(AreEqual(Real(0.051672494248), v2[0]));
      Assert::IsTrue(AreEqual(Real(0.096972407865), v2[1]));
      Assert::IsTrue(AreEqual(Real(0.969534531337), v2[2]));
      Assert::IsTrue(AreEqual(Real(0.218927152237), v2[3]));
    }

    TEST_METHOD(Vector4ZeroEpsilon)
    {
      Vector4 v(SF_EPSILON / 5, SF_EPSILON / 5, SF_EPSILON / 5, SF_EPSILON / 5);

      Assert::IsTrue(v.IsZeroEpsilon());

      Vector4 v1(Real(-0.231), Real(142.23), Real(-234.3), Real(222.1));
      Assert::IsFalse(v1.IsZeroEpsilon());
    }

    TEST_METHOD(Vector4AddOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v2(Real(-0.1), Real(2.1), Real(-10.33), Real(5.3213));
      Vector4 r;

      r = v1 + v2;

      Assert::IsTrue(AreEqual(Real(5.1), r[0]));
      Assert::IsTrue(AreEqual(Real(-1.0), r[1]));
      Assert::IsTrue(AreEqual(Real(3.0121), r[2]));
      Assert::IsTrue(AreEqual(Real(8.32129), r[3]));

      v1 += v2;
      Assert::IsTrue(AreEqual(Real(5.1), v1[0]));
      Assert::IsTrue(AreEqual(Real(-1.0), v1[1]));
      Assert::IsTrue(AreEqual(Real(3.0121), v1[2]));
      Assert::IsTrue(AreEqual(Real(8.32129), v1[3]));
    }

    TEST_METHOD(Vector4NegOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 r;

      r = -v1;

      Assert::IsTrue(AreEqual(Real(-5.2), r[0]));
      Assert::IsTrue(AreEqual(Real(3.1), r[1]));
      Assert::IsTrue(AreEqual(Real(-13.3421), r[2]));
      Assert::IsTrue(AreEqual(Real(-2.99999), r[3]));

      Vector4 v2(Real(-1.0), Real(1.0), Real(-1.0), Real(-1.0));
      r = -v2;
      Assert::IsTrue(AreEqual(Real(1.0), r[0]));
      Assert::IsTrue(AreEqual(Real(-1.0), r[1]));
      Assert::IsTrue(AreEqual(Real(1.0), r[2]));
      Assert::IsTrue(AreEqual(Real(1.0), r[3]));
    }

    TEST_METHOD(Vector4SubOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v2(Real(-0.1), Real(2.1), Real(-10.33), Real(5.3213));
      Vector4 r;

      r = v1 - v2;

      Assert::IsTrue(AreEqual(Real(5.3), r[0]));
      Assert::IsTrue(AreEqual(Real(-5.2), r[1]));
      Assert::IsTrue(AreEqual(Real(23.6721), r[2]));
      Assert::IsTrue(AreEqual(Real(-2.32131), r[3]));

      v1 -= v2;
      Assert::IsTrue(AreEqual(Real(5.3), v1[0]));
      Assert::IsTrue(AreEqual(Real(-5.2), v1[1]));
      Assert::IsTrue(AreEqual(Real(23.6721), v1[2]));
      Assert::IsTrue(AreEqual(Real(-2.32131), v1[3]));
    }

    TEST_METHOD(VEctor4ScalarMulOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v2(Real(0.0), Real(0.0), Real(0.0), Real(0.0));
      Vector4 r;

      r = v1 * Real(2.2);
      Assert::IsTrue(AreEqual(Real(11.44), r[0]));
      Assert::IsTrue(AreEqual(Real(-6.82), r[1]));
      Assert::IsTrue(AreEqual(Real(29.3526211), r[2]));
      Assert::IsTrue(AreEqual(Real(6.599978), r[3]));

      v1 *= Real(2.2);
      Assert::IsTrue(AreEqual(Real(11.44), v1[0]));
      Assert::IsTrue(AreEqual(Real(-6.82), v1[1]));
      Assert::IsTrue(AreEqual(Real(29.3526211), v1[2]));
      Assert::IsTrue(AreEqual(Real(6.599978), v1[3]));

      r = v2 * Real(12412.2213);
      Assert::IsTrue(AreEqual(Real(0.0), r[0]));
      Assert::IsTrue(AreEqual(Real(0.0), r[1]));
      Assert::IsTrue(AreEqual(Real(0.0), r[2]));
      Assert::IsTrue(AreEqual(Real(0.0), r[3]));

      v2 *= Real(3123.34211);
      Assert::IsTrue(AreEqual(Real(0.0), v2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[1]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[2]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[3]));
    }

    TEST_METHOD(Vector4ScalarDivOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v2(Real(0.0), Real(0.0), Real(0.0), Real(0.0));
      Vector4 r;

      r = v1 / Real(0.786);
      Assert::IsTrue(AreEqual(Real(6.61577608142), r[0]));
      Assert::IsTrue(AreEqual(Real(-3.94402035623), r[1]));
      Assert::IsTrue(AreEqual(Real(16.9746819338), r[2]));
      Assert::IsTrue(AreEqual(Real(3.81678117048), r[3]));

      v1 /= Real(0.786);
      Assert::IsTrue(AreEqual(Real(6.61577608142), v1[0]));
      Assert::IsTrue(AreEqual(Real(-3.94402035623), v1[1]));
      Assert::IsTrue(AreEqual(Real(16.9746819338), v1[2]));
      Assert::IsTrue(AreEqual(Real(3.81678117048), v1[3]));

      v2 /= Real(0.34211);
      Assert::IsTrue(AreEqual(Real(0.0), v2[0]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[1]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[2]));
      Assert::IsTrue(AreEqual(Real(0.0), v2[3]));
    }

    TEST_METHOD(Vector4IndOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));


      Assert::IsTrue(AreEqual(Real(5.2), v1[0]));
      Assert::IsTrue(AreEqual(Real(-3.1), v1[1]));
      Assert::IsTrue(AreEqual(Real(13.3421), v1[2]));
      Assert::IsTrue(AreEqual(Real(2.99999), v1[3]));
    }

    TEST_METHOD(Vector4EqOp)
    {
      Vector4 v1(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v2(Real(0.0), Real(0.0), Real(0.0), Real(0.0));
      Vector4 v3(Real(5.2), Real(-3.1), Real(13.3421), Real(2.8));
      Vector4 v4(Real(5.2), Real(-3.1), Real(-13.3421), Real(2.99999));
      Vector4 v5(Real(5.2), Real(3.1), Real(13.3421), Real(2.99999));
      Vector4 v6(Real(-5.2), Real(-3.1), Real(13.3421), Real(2.99999));
      Vector4 v7(Real(5.2), Real(-3.1), Real(13.3421), Real(2.99999));

      Assert::IsTrue(v1 == v7);
      Assert::IsFalse(v1 != v7);

      Assert::IsFalse(v1 == v2);
      Assert::IsTrue(v1 != v2);

      Assert::IsTrue(v1 != v3);
      Assert::IsTrue(v1 != v4);
      Assert::IsTrue(v1 != v5);
      Assert::IsTrue(v1 != v6);

      Assert::IsFalse(v1 == v3);
      Assert::IsFalse(v1 == v4);
      Assert::IsFalse(v1 == v5);
      Assert::IsFalse(v1 == v6);
    }
  };
}