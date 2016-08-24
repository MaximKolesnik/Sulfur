#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/Engine/Math/sfMathDefs.hpp"
#include "../src/Engine/Math/sfVector3.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

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
    TEST_METHOD(Vector3Set0)
    {
      Sulfur::Vector3 v(1.0, 2.2, 3.999);

      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(2.2), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.999), v[2]));
    }

    TEST_METHOD(Vector3Set1)
    {
      Sulfur::Vector3 v;
      v.Set(1.0, 2.2, 3.999);

      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(2.2), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.999), v[2]));

      v.SetX(-5.32);
      v.SetY(14.213);
      v.SetZ(5.2);

      Assert::IsTrue(AreEqual(Sulfur::Real(-5.32), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(14.213), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(5.2), v[2]));
    }

    TEST_METHOD(Vector3OpEq)
    {
      Sulfur::Vector3 v1;
      Sulfur::Vector3 v2(1.0, 2.2, 3.999);
      v1 = v2;

      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v1[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(2.2), v1[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.999), v1[2]));
    }

    TEST_METHOD(Vector3Get)
    {
      Sulfur::Vector3 v(1.0, 2.2, 3.999);

      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v.GetX()));
      Assert::IsTrue(AreEqual(Sulfur::Real(2.2), v.GetY()));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.999), v.GetZ()));
    }

    TEST_METHOD(Vector3Dot)
    {
      Sulfur::Vector3 v1(0.2, 3.5, 2.1);
      Sulfur::Vector3 v2(-0.5, 6.319, -1.22);

      Assert::IsTrue(AreEqual(Sulfur::Real(19.454498), v1.Dot(v2)));
      Assert::IsTrue(AreEqual(Sulfur::Real(19.454498), Dot(v1, v2)));
    }

    TEST_METHOD(Vector3Cross)
    {
      Sulfur::Vector3 v1(0.2, 3.5, 2.1);
      Sulfur::Vector3 v2(-0.5, 6.319, -1.22);
      Sulfur::Vector3 v = v1.Cross(v2);
      Sulfur::Vector3 vc = Sulfur::Cross(v1, v2);

      Assert::IsTrue(AreEqual(Sulfur::Real(-17.5399), v.GetX()));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.806), v.GetY()));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.01380014), v.GetZ()));

      Assert::IsTrue(AreEqual(Sulfur::Real(-17.5399), vc.GetX()));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.806), vc.GetY()));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.01380014), vc.GetZ()));
    }

    TEST_METHOD(Vector3Length)
    {
      Sulfur::Vector3 v(0.25, 3.456, 2.5);

      Assert::IsTrue(AreEqual(Sulfur::Real(18.256436), v.LengthSq()));
      Assert::IsTrue(AreEqual(Sulfur::Real(18.256436), Sulfur::LengthSq(v)));
      Assert::IsTrue(AreEqual(Sulfur::Real(4.27275508308), v.Length()));
      Assert::IsTrue(AreEqual(Sulfur::Real(4.27275508308), Sulfur::Length(v)));
    }

    TEST_METHOD(Vector3Normalize)
    {
      Sulfur::Vector3 v(0.7321, -2.333, 3.0);
      Sulfur::Vector3 vn = v.Normalized();

      Assert::IsTrue(AreEqual(Sulfur::Real(0.189160753182), vn[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.602802946555), vn[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.775143094584), vn[2]));

      v.Normalize();
      Assert::IsTrue(AreEqual(Sulfur::Real(0.189160753182), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.602802946555), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.775143094584), v[2]));

      v.Set(0.7321, -2.333, 3.0);
      vn = Sulfur::Normalize(v);
      Assert::IsTrue(AreEqual(Sulfur::Real(0.189160753182), vn[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.602802946555), vn[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.775143094584), vn[2]));
    }

    TEST_METHOD(Vector3ZeroOut)
    {
      Sulfur::Vector3 v(0.1, -6.435, 3.232);

      v.ZeroOut();
      Assert::AreEqual(Sulfur::Real(0.0), v[0]);
      Assert::AreEqual(Sulfur::Real(0.0), v[1]);
      Assert::AreEqual(Sulfur::Real(0.0), v[2]);
    }

    TEST_METHOD(Vector3IsZeroEpsilon)
    {
      Sulfur::Vector3 v(0.00000001, 0.00000001, 0.00000001);

      Assert::IsTrue(v.IsZeroEpsilon());
    }

    TEST_METHOD(Vector3HasInfiniry)
    {
      Sulfur::Vector3 v(0.0, 0.0, 0.0);

      Assert::IsFalse(v.HasInfinity());

      Sulfur::Real inf = std::numeric_limits<Sulfur::Real>::infinity();
      v.Set(inf, inf, inf);

      Assert::IsTrue(v.HasInfinity());

      v.Set(0.0, inf, inf);
      Assert::IsTrue(v.HasInfinity());

      v.Set(0.0, 0.0, inf);
      Assert::IsTrue(v.HasInfinity());

      v.Set(inf, 0.0, inf);
      Assert::IsTrue(v.HasInfinity());

      v.Set(inf, inf, 0.0);
      Assert::IsTrue(v.HasInfinity());

      v.Set(inf, 0.0, 0.0);
      Assert::IsTrue(v.HasInfinity());

      v.Set(0.0, inf, 0.0);
      Assert::IsTrue(v.HasInfinity());
    }

    TEST_METHOD(Vector3Abs)
    {
      Sulfur::Vector3 v(-0.2341, 5.1257, -13.231);
      Sulfur::Vector3 va = v.GetAbs();

      Assert::AreEqual(Sulfur::Real(0.2341), va[0]);
      Assert::AreEqual(Sulfur::Real(5.1257), va[1]);
      Assert::AreEqual(Sulfur::Real(13.231), va[2]);

      v.Abs();
      Assert::AreEqual(Sulfur::Real(0.2341), v[0]);
      Assert::AreEqual(Sulfur::Real(5.1257), v[1]);
      Assert::AreEqual(Sulfur::Real(13.231), v[2]);

      v.Set(-0.2341, 5.1257, -13.231);
      Sulfur::Abs(v);
      Assert::AreEqual(Sulfur::Real(0.2341), v[0]);
      Assert::AreEqual(Sulfur::Real(5.1257), v[1]);
      Assert::AreEqual(Sulfur::Real(13.231), v[2]);
    }

    TEST_METHOD(Vector3Neg)
    {
      Sulfur::Vector3 v(-1.5436, -10.243, 4.321);
      Sulfur::Vector3 vn = v.Negated();

      Assert::AreEqual(Sulfur::Real(1.5436), vn[0]);
      Assert::AreEqual(Sulfur::Real(10.243), vn[1]);
      Assert::AreEqual(Sulfur::Real(-4.321), vn[2]);

      v.Negate();
      Assert::AreEqual(Sulfur::Real(1.5436), v[0]);
      Assert::AreEqual(Sulfur::Real(10.243), v[1]);
      Assert::AreEqual(Sulfur::Real(-4.321), v[2]);

      v.Set(-1.5436, -10.243, 4.321);
      Sulfur::Negate(v);
      Assert::AreEqual(Sulfur::Real(1.5436), v[0]);
      Assert::AreEqual(Sulfur::Real(10.243), v[1]);
      Assert::AreEqual(Sulfur::Real(-4.321), v[2]);
    }

    TEST_METHOD(Vector3Distance)
    {
      Sulfur::Vector3 v1(-2.5, 3.0, 7.0);
      Sulfur::Vector3 v2(2.5, 0.0, 0.0);

      Assert::AreEqual(Sulfur::Real(9.11043357914), v1.Distance(v2));
      Assert::AreEqual(Sulfur::Real(83), v1.DistanceSq(v2));
      Assert::AreEqual(Sulfur::Real(9.11043357914), Sulfur::Distance(v1, v2));
      Assert::AreEqual(Sulfur::Real(83), Sulfur::DistanceSq(v1, v2));
    }

    TEST_METHOD(Vector3MinMaxAxis)
    {
      Sulfur::Vector3 v(-2.5, 3.0, 1.0);

      Assert::AreEqual(1, v.MaxAxis());
      Assert::AreEqual(0, v.MinAxis());
      Assert::AreEqual(Sulfur::Real(3.0), v.MaxAxisValue());
      Assert::AreEqual(Sulfur::Real(-2.5), v.MinAxisValue());

      v.Set(3.0, -2.5, 1.0);
      Assert::AreEqual(0, v.MaxAxis());
      Assert::AreEqual(1, v.MinAxis());
      Assert::AreEqual(Sulfur::Real(3.0), v.MaxAxisValue());
      Assert::AreEqual(Sulfur::Real(-2.5), v.MinAxisValue());

      v.Set(0.0, 1.0, 2.0);
      Assert::AreEqual(2, v.MaxAxis());
      Assert::AreEqual(0, v.MinAxis());
      Assert::AreEqual(Sulfur::Real(2.0), v.MaxAxisValue());
      Assert::AreEqual(Sulfur::Real(0.0), v.MinAxisValue());

      Assert::AreEqual(2, Sulfur::MaxAxis(v));
      Assert::AreEqual(0, Sulfur::MinAxis(v));
      Assert::AreEqual(Sulfur::Real(2.0), Sulfur::MaxAxisValue(v));
      Assert::AreEqual(Sulfur::Real(0.0), Sulfur::MinAxisValue(v));
    }

    TEST_METHOD(Vector3Lerp)
    {
      Sulfur::Vector3 v1(0.0, 3.0, 7.0);
      Sulfur::Vector3 v2(-3.0, 5.0, -3.0);
      Sulfur::Vector3 lerped = v1.Lerp(v2, 0.5);

      Assert::AreEqual(Sulfur::Real(-1.5), lerped[0]);
      Assert::AreEqual(Sulfur::Real(4.0), lerped[1]);
      Assert::AreEqual(Sulfur::Real(2.0), lerped[2]);

      lerped = Sulfur::Lerp(v1, v2, 0.5);

      Assert::AreEqual(Sulfur::Real(-1.5), lerped[0]);
      Assert::AreEqual(Sulfur::Real(4.0), lerped[1]);
      Assert::AreEqual(Sulfur::Real(2.0), lerped[2]);
    }

    TEST_METHOD(Vector3Rotate)
    {
      Sulfur::Vector3 v(1.0, 0.0, 0.0);
      Sulfur::Vector3 axis(0.0, 0.0, 1.0);
      Sulfur::Vector3 vr = v.RotatedRad(axis, SF_PI / 2);

      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), vr[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), vr[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), vr[2]));

      v.Set(0.5, 0.0, 0.34);
      axis.Set(0.75, 0.245, 1.0);
      axis.Normalize();
      vr = Sulfur::RotateDeg(v, axis, 45.0);
      Assert::IsTrue(AreEqual(Sulfur::Real(0.49659735), vr[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.16762721), vr[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.30148333), vr[2]));
    }

    TEST_METHOD(Vector3GetAngle)
    {
      Sulfur::Vector3 v1(1.0, 0.0, 0.0);
      Sulfur::Vector3 v2(0.0, 1.0, 0.0);

      Assert::IsTrue(AreEqual(Sulfur::Real(SF_PI / 2), v1.GetAngle(v2)));
      
      v1.Set(1.325, -0.342, 3.98);
      v2.Set(-0.231, 2.2355, 2.5);

      Assert::IsTrue(AreEqual(Sulfur::Real(0.89233154), v1.GetAngle(v2)));
    }

    TEST_METHOD(Vector3OpAdd)
    {
      Sulfur::Vector3 v1(1.2345, -0.9876, 4.435435);
      Sulfur::Vector3 v2(-0.12313, 0.4234, 0.1313);
      Sulfur::Vector3 res = v1 + v2;

      Assert::IsTrue(AreEqual(Sulfur::Real(1.11137009), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.564200044), res[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(4.56673479), res[2]));

      v1 += v2;

      Assert::IsTrue(AreEqual(Sulfur::Real(1.11137009), v1[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-0.564200044), v1[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(4.56673479), v1[2]));
    }

    TEST_METHOD(Vector3OpNeg)
    {
      Sulfur::Vector3 v(1.0, -0.5, 3.45);

      v = -v;

      Assert::AreEqual(Sulfur::Real(-1.0), v[0]);
      Assert::AreEqual(Sulfur::Real(0.5), v[1]);
      Assert::AreEqual(Sulfur::Real(-3.45), v[2]);
    }

    TEST_METHOD(Vector3OpSub)
    {
      Sulfur::Vector3 v1(1.2345, -0.9876, 4.435435);
      Sulfur::Vector3 v2(-0.12313, 0.4234, 0.1313);
      Sulfur::Vector3 res = v1 - v2;

      Assert::AreEqual(Sulfur::Real(1.35763), res[0]);
      Assert::AreEqual(Sulfur::Real(-1.411), res[1]);
      Assert::AreEqual(Sulfur::Real(4.304135), res[2]);

      v1 -= v2;

      Assert::AreEqual(Sulfur::Real(1.35763), v1[0]);
      Assert::AreEqual(Sulfur::Real(-1.411), v1[1]);
      Assert::AreEqual(Sulfur::Real(4.304135), v1[2]);
    }

    TEST_METHOD(Vector3OpScaling)
    {
      Sulfur::Vector3 v(0.5, 23.16785, -3.5632);
      Sulfur::Vector3 res = v * 2.5;

      Assert::IsTrue(AreEqual(Sulfur::Real(1.25), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(57.919625), res[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-8.908), res[2]));
  
      v *= 2.5;

      Assert::IsTrue(AreEqual(Sulfur::Real(1.25), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(57.919625), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-8.908), v[2]));

      v.Set(-1.5344, -5.24241, 9.4643);
      res = v / 1.5;

      Assert::IsTrue(AreEqual(Sulfur::Real(-1.022933333333), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-3.49494), res[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(6.30953360), res[2]));

      v /= 1.5;

      Assert::IsTrue(AreEqual(Sulfur::Real(-1.022933333333), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-3.49494), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(6.30953360), v[2]));
    }

    TEST_METHOD(Vector3OpEquals)
    {
      Sulfur::Vector3 v1(0.315, -45.1475, 9.31155);
      Sulfur::Vector3 v2(0.315, -45.1475, 9.31155);
      Sulfur::Vector3 v3(0.5322, 5.426731, 0.4424);

      Assert::IsTrue(v1 == v2);
      Assert::IsTrue(v2 != v3);
      Assert::IsTrue(v1 != v3);
    }
  };
}