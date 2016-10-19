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
      Vector4 vv(_mm_set_ps(-5.232, 123.332, 0.001, -23.4543));

      Assert::IsTrue(AreEqual(Real(-23.4543), vv.GetX()));
      Assert::IsTrue(AreEqual(Real(0.001), vv.GetY()));
      Assert::IsTrue(AreEqual(Real(123.332), vv.GetZ()));
      Assert::IsTrue(AreEqual(Real(-5.232), vv.GetW()));
#endif
    }

    TEST_METHOD(Vector4AssignOp)
    {
      Vector4 v(Real(0.123), Real(-12.32), Real(32.2), Real(55.1234));
      Vector4 vv(-5.232, 123.332, 0.001, -23.4543);

      v = vv;

      Assert::IsTrue(AreEqual(Real(-23.4543), v.GetX()));
      Assert::IsTrue(AreEqual(Real(0.001), v.GetY()));
      Assert::IsTrue(AreEqual(Real(123.332), v.GetZ()));
      Assert::IsTrue(AreEqual(Real(-5.232), v.GetW()));
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
      v.Set(_mm_set_ps(-5.232, 123.332, 0.001, -23.4543));

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
      Vector4 v1(-0.231, 142.23, -234.3, 222.1);
      Vector4 v2(5.4522, 10.232, 102.3, 23.1);

      d = v1.Dot(v2);
      Assert::IsTrue(AreEqual(Real(-17384.34209882), d));

      d = Dot(v1, v2);
      Assert::IsTrue(AreEqual(Real(-17384.34209882), d));

      v1.Set(0.2, 1.3, 3.21, -2.3);
      v2.Set(-1.5, -5.2, 4.1, -9.9);

      d = v1.Dot(v2);
      Assert::IsTrue(AreEqual(Real(28.871), d));

      d = Dot(v1, v2);
      Assert::IsTrue(AreEqual(Real(28.871), d));

      Vector3 v3(1.2, -5.4, 3.0);

      d = v1.Dot(v3);
      Assert::IsTrue(AreEqual(Real(0.55), d));

      d = Dot(v1, v3);
      Assert::IsTrue(AreEqual(Real(0.55), d));
    }

    TEST_METHOD(Vector4Length)
    {

    }
  };
}