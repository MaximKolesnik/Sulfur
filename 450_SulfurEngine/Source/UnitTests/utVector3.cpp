/******************************************************************************/
/*!
\par     Sulfur
\file    utVector3.cpp
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
#include "Math/sfVector3.hpp"

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
    TEST_METHOD(Vector3Set0)
    {
      Vector3 v(Real(1.0), Real(2.2), Real(3.999));

      Assert::IsTrue(AreEqual(Real(1.0), v[0]));
      Assert::IsTrue(AreEqual(Real(2.2), v[1]));
      Assert::IsTrue(AreEqual(Real(3.999), v[2]));
    }

    TEST_METHOD(Vector3Set1)
    {
      Vector3 v;
      v.Set(Real(1.0), Real(2.2f), Real(3.999f));

      Assert::IsTrue(AreEqual(Real(1.0), v[0]));
      Assert::IsTrue(AreEqual(Real(2.2), v[1]));
      Assert::IsTrue(AreEqual(Real(3.999), v[2]));

      v.SetX(Real(-5.32));
      v.SetY(Real(14.213));
      v.SetZ(Real(5.2));

      Assert::IsTrue(AreEqual(Real(-5.32), v[0]));
      Assert::IsTrue(AreEqual(Real(14.213), v[1]));
      Assert::IsTrue(AreEqual(Real(5.2), v[2]));
    }

    TEST_METHOD(Vector3OpEq)
    {
      Vector3 v1;
      Vector3 v2(Real(1.0f), 
        Real(2.2f), Real(3.999));
      v1 = v2;

      Assert::IsTrue(AreEqual(Real(1.0), v1[0]));
      Assert::IsTrue(AreEqual(Real(2.2), v1[1]));
      Assert::IsTrue(AreEqual(Real(3.999), v1[2]));
    }

    TEST_METHOD(Vector3Get)
    {
      Vector3 v(Real(1.0), Real(2.2), Real(3.999));

      Assert::IsTrue(AreEqual(Real(1.0), v.GetX()));
      Assert::IsTrue(AreEqual(Real(2.2), v.GetY()));
      Assert::IsTrue(AreEqual(Real(3.999), v.GetZ()));
    }

    TEST_METHOD(Vector3Dot)
    {
      Vector3 v1(Real(0.2), Real(3.5), Real(2.1));
      Vector3 v2(Real(-0.5), Real(6.319), Real(-1.22));

      Assert::IsTrue(AreEqual(Real(19.454498), v1.Dot(v2)));
      Assert::IsTrue(AreEqual(Real(19.454498), Dot(v1, v2)));
    }

    TEST_METHOD(Vector3Cross)
    {
      Vector3 v1(Real(0.2), Real(3.5), Real(2.1));
      Vector3 v2(Real(-0.5), Real(6.319), Real(-1.22));
      Vector3 v = v1.Cross(v2);
      Vector3 vc = Cross(v1, v2);

      Assert::IsTrue(AreEqual(Real(-17.5399), v.GetX()));
      Assert::IsTrue(AreEqual(Real(-0.806), v.GetY()));
      Assert::IsTrue(AreEqual(Real(3.01380014), v.GetZ()));

      Assert::IsTrue(AreEqual(Real(-17.5399), vc.GetX()));
      Assert::IsTrue(AreEqual(Real(-0.806), vc.GetY()));
      Assert::IsTrue(AreEqual(Real(3.01380014), vc.GetZ()));
    }

    TEST_METHOD(Vector3Length)
    {
      Vector3 v(Real(0.25), Real(3.456), Real(2.5));

      Assert::IsTrue(AreEqual(Real(18.256436), v.LengthSq()));
      Assert::IsTrue(AreEqual(Real(18.256436), LengthSq(v)));
      Assert::IsTrue(AreEqual(Real(4.27275508308), v.Length()));
      Assert::IsTrue(AreEqual(Real(4.27275508308), Length(v)));
    }

    TEST_METHOD(Vector3Normalize)
    {
      Vector3 v(Real(0.7321), Real(-2.333), Real(3.0));
      Vector3 vn = v.Normalized();

      Assert::IsTrue(AreEqual(Real(0.189160753182), vn[0]));
      Assert::IsTrue(AreEqual(Real(-0.602802946555), vn[1]));
      Assert::IsTrue(AreEqual(Real(0.775143094584), vn[2]));

      v.Normalize();
      Assert::IsTrue(AreEqual(Real(0.189160753182), v[0]));
      Assert::IsTrue(AreEqual(Real(-0.602802946555), v[1]));
      Assert::IsTrue(AreEqual(Real(0.775143094584), v[2]));

      v.Set(Real(0.7321), Real(-2.333), Real(3.0));
      vn = Normalize(v);
      Assert::IsTrue(AreEqual(Real(0.189160753182), vn[0]));
      Assert::IsTrue(AreEqual(Real(-0.602802946555), vn[1]));
      Assert::IsTrue(AreEqual(Real(0.775143094584), vn[2]));
    }

    TEST_METHOD(Vector3ZeroOut)
    {
      Vector3 v(Real(0.1), Real(-6.435), 
        Real(3.232));

      v.ZeroOut();
      Assert::AreEqual(Real(0.0), v[0]);
      Assert::AreEqual(Real(0.0), v[1]);
      Assert::AreEqual(Real(0.0), v[2]);
    }

    TEST_METHOD(Vector3IsZeroEpsilon)
    {
      Vector3 v(Real(0.00000001), Real(0.00000001), 
        Real(0.00000001));

      Assert::IsTrue(v.IsZeroEpsilon());
    }

    TEST_METHOD(Vector3HasInfiniry)
    {
      Vector3 v(Real(0.0), Real(0.0), 
        Real(0.0));

      Assert::IsFalse(v.HasInfinity());

      Real inf = std::numeric_limits<Real>::infinity();
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
      Vector3 v(Real(-0.2341), 
        Real(5.1257), Real(-13.231));

      Vector3 va = v.GetAbs();

      Assert::AreEqual(Real(0.2341), va[0]);
      Assert::AreEqual(Real(5.1257), va[1]);
      Assert::AreEqual(Real(13.231), va[2]);

      v.Abs();
      Assert::AreEqual(Real(0.2341), v[0]);
      Assert::AreEqual(Real(5.1257), v[1]);
      Assert::AreEqual(Real(13.231), v[2]);

      v.Set(Real(-0.2341), Real(5.1257), Real(-13.231));
      Abs(v);
      Assert::AreEqual(Real(0.2341), v[0]);
      Assert::AreEqual(Real(5.1257), v[1]);
      Assert::AreEqual(Real(13.231), v[2]);
    }

    TEST_METHOD(Vector3Neg)
    {
      Vector3 v(Real(-1.5436), Real(-10.243), 
        Real(4.321));
      Vector3 vn = v.Negated();

      Assert::AreEqual(Real(1.5436), vn[0]);
      Assert::AreEqual(Real(10.243), vn[1]);
      Assert::AreEqual(Real(-4.321), vn[2]);

      v.Negate();
      Assert::AreEqual(Real(1.5436), v[0]);
      Assert::AreEqual(Real(10.243), v[1]);
      Assert::AreEqual(Real(-4.321), v[2]);

      v.Set(Real(-1.5436), Real(-10.243), 
        Real(4.321));
      Negate(v);
      Assert::AreEqual(Real(1.5436), v[0]);
      Assert::AreEqual(Real(10.243), v[1]);
      Assert::AreEqual(Real(-4.321), v[2]);
    }

    TEST_METHOD(Vector3Distance)
    {
      Vector3 v1(Real(-2.5), Real(3.0), Real(7.0));
      Vector3 v2(Real(2.5), Real(0.0), Real(0.0));

      Assert::AreEqual(Real(9.11043357914), v1.Distance(v2));
      Assert::AreEqual(Real(83), v1.DistanceSq(v2));
      Assert::AreEqual(Real(9.11043357914), Distance(v1, v2));
      Assert::AreEqual(Real(83), DistanceSq(v1, v2));
    }

    TEST_METHOD(Vector3MinMaxAxis)
    {
      Vector3 v(Real(-2.5), Real(3.0), Real(1.0));

      Assert::AreEqual(1, v.MaxAxis());
      Assert::AreEqual(0, v.MinAxis());
      Assert::AreEqual(Real(3.0), v.MaxAxisValue());
      Assert::AreEqual(Real(-2.5), v.MinAxisValue());

      v.Set(Real(3.0), Real(-2.5), Real(1.0));
      Assert::AreEqual(0, v.MaxAxis());
      Assert::AreEqual(1, v.MinAxis());
      Assert::AreEqual(Real(3.0), v.MaxAxisValue());
      Assert::AreEqual(Real(-2.5), v.MinAxisValue());

      v.Set(Real(0.0), Real(1.0), Real(2.0));
      Assert::AreEqual(2, v.MaxAxis());
      Assert::AreEqual(0, v.MinAxis());
      Assert::AreEqual(Real(2.0), v.MaxAxisValue());
      Assert::AreEqual(Real(0.0), v.MinAxisValue());

      Assert::AreEqual(2, MaxAxis(v));
      Assert::AreEqual(0, MinAxis(v));
      Assert::AreEqual(Real(2.0), MaxAxisValue(v));
      Assert::AreEqual(Real(0.0), MinAxisValue(v));
    }

    TEST_METHOD(Vector3Lerp)
    {
      Vector3 v1(Real(0.0), Real(3.0), Real(7.0));
      Vector3 v2(Real(-3.0), Real(5.0), Real(-3.0));
      Vector3 lerped = v1.Lerp(v2, Real(0.5));

      Assert::AreEqual(Real(-1.5), lerped[0]);
      Assert::AreEqual(Real(4.0), lerped[1]);
      Assert::AreEqual(Real(2.0), lerped[2]);

      lerped = Lerp(v1, v2, Real(0.5));

      Assert::AreEqual(Real(-1.5), lerped[0]);
      Assert::AreEqual(Real(4.0), lerped[1]);
      Assert::AreEqual(Real(2.0), lerped[2]);
    }

    TEST_METHOD(Vector3Rotate)
    {
      Vector3 v(Real(1.0), Real(0.0), Real(0.0));
      Vector3 axis(Real(0.0), Real(0.0), Real(1.0));
      Vector3 vr = v.RotatedRad(axis, SF_PI / 2);

      Assert::IsTrue(AreEqual(Real(0.0), vr[0]));
      Assert::IsTrue(AreEqual(Real(1.0), vr[1]));
      Assert::IsTrue(AreEqual(Real(0.0), vr[2]));

      v.Set(Real(0.5), Real(0.0), Real(0.34));
      axis.Set(Real(0.75), Real(0.245), Real(1.0));
      axis.Normalize();
      vr = RotateDeg(v, axis, Real(45.0));
      Assert::IsTrue(AreEqual(Real(0.49659735), vr[0]));
      Assert::IsTrue(AreEqual(Real(0.16762721), vr[1]));
      Assert::IsTrue(AreEqual(Real(0.30148333), vr[2]));
    }

    TEST_METHOD(Vector3GetAngle)
    {
      Vector3 v1(Real(1.0), Real(0.0), Real(0.0));
      Vector3 v2(Real(0.0), Real(1.0), Real(0.0));

      Assert::IsTrue(AreEqual(Real(SF_PI / 2), v1.GetAngle(v2)));
      
      v1.Set(Real(1.325), Real(-0.342), Real(3.98));
      v2.Set(Real(-0.231), Real(2.2355), Real(2.5));

      Assert::IsTrue(AreEqual(Real(0.89233154), v1.GetAngle(v2)));
    }

    TEST_METHOD(Vector3OpAdd)
    {
      Vector3 v1(Real(1.2345), Real(-0.9876), Real(4.435435));
      Vector3 v2(Real(-0.12313), Real(0.4234), Real(0.1313));
      Vector3 res = v1 + v2;

      Assert::IsTrue(AreEqual(Real(1.11137009), res[0]));
      Assert::IsTrue(AreEqual(Real(-0.564200044), res[1]));
      Assert::IsTrue(AreEqual(Real(4.56673479), res[2]));

      v1 += v2;

      Assert::IsTrue(AreEqual(Real(1.11137009), v1[0]));
      Assert::IsTrue(AreEqual(Real(-0.564200044), v1[1]));
      Assert::IsTrue(AreEqual(Real(4.56673479), v1[2]));
    }

    TEST_METHOD(Vector3OpNeg)
    {
      Vector3 v(Real(1.0), Real(-0.5), Real(3.45));

      v = -v;

      Assert::AreEqual(Real(-1.0), v[0]);
      Assert::AreEqual(Real(0.5), v[1]);
      Assert::AreEqual(Real(-3.45), v[2]);
    }

    TEST_METHOD(Vector3OpSub)
    {
      Vector3 v1(Real(1.2345), Real(-0.9876), Real(4.435435));
      Vector3 v2(Real(-0.12313), Real(0.4234), Real(0.1313));
      Vector3 res = v1 - v2;

      Assert::AreEqual(Real(1.35763), res[0]);
      Assert::AreEqual(Real(-1.411), res[1]);
      Assert::AreEqual(Real(4.304135), res[2]);

      v1 -= v2;

      Assert::AreEqual(Real(1.35763), v1[0]);
      Assert::AreEqual(Real(-1.411), v1[1]);
      Assert::AreEqual(Real(4.304135), v1[2]);
    }

    TEST_METHOD(Vector3OpScaling)
    {
      Vector3 v(Real(0.5), Real(23.16785), Real(-3.5632));
      Vector3 res = v * 2.5;

      Assert::IsTrue(AreEqual(Real(1.25), res[0]));
      Assert::IsTrue(AreEqual(Real(57.919625), res[1]));
      Assert::IsTrue(AreEqual(Real(-8.908), res[2]));
  
      v *= 2.5;

      Assert::IsTrue(AreEqual(Real(1.25), v[0]));
      Assert::IsTrue(AreEqual(Real(57.919625), v[1]));
      Assert::IsTrue(AreEqual(Real(-8.908), v[2]));

      v.Set(Real(-1.5344), Real(-5.24241), Real(9.4643));
      res = v / 1.5;

      Assert::IsTrue(AreEqual(Real(-1.022933333333), res[0]));
      Assert::IsTrue(AreEqual(Real(-3.49494), res[1]));
      Assert::IsTrue(AreEqual(Real(6.30953360), res[2]));

      v /= 1.5;

      Assert::IsTrue(AreEqual(Real(-1.022933333333), v[0]));
      Assert::IsTrue(AreEqual(Real(-3.49494), v[1]));
      Assert::IsTrue(AreEqual(Real(6.30953360), v[2]));
    }

    TEST_METHOD(Vector3OpEquals)
    {
      Vector3 v1(Real(0.315), Real(-45.1475), Real(9.31155));
      Vector3 v2(Real(0.315), Real(-45.1475), Real(9.31155));
      Vector3 v3(Real(0.5322), Real(5.426731), Real(0.4424));

      Assert::IsTrue(v1 == v2);
      Assert::IsTrue(v2 != v3);
      Assert::IsTrue(v1 != v3);
    }
  };
}