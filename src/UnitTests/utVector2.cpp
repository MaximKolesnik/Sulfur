/******************************************************************************/
/*!
\par     Sulfur
\file    utVector2.cpp
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
#include "Math/sfVector2.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#pragma warning(disable : 4305)

namespace UnitTests
{		
  template<typename T>
  static bool AreEqual(const T &expected, const T &actual)
  {
    if ( std::abs(expected - actual) < SF_UT_EPSILON)
      return true;

    return false;
  }

	TEST_CLASS(utVector3)
	{
	public:
		
		TEST_METHOD(Vector2Set0)
		{
			Sulfur::Vector2 v0(3.0, 2.0);

      Assert::AreEqual(Sulfur::Real(3.0), v0[0]);
      Assert::AreEqual(Sulfur::Real(2.0), v0[1]);
		}

    TEST_METHOD(Vector2Set1)
    {
      Sulfur::Vector2 v(0.0, 0.0);

      Assert::AreEqual(Sulfur::Real(0.0), v[0]);
      Assert::AreEqual(Sulfur::Real(0.0), v[1]);
    }
    
    TEST_METHOD(Vector2Get0)
    {
      Sulfur::Vector2 v(2.65, 0.003);

      Assert::AreEqual(Sulfur::Real(2.65), v.GetX());
      Assert::AreEqual(Sulfur::Real(0.003), v.GetY());
    }

    TEST_METHOD(Vector2Get1)
    {
      Sulfur::Vector2 v(0.0, 0.0);

      Assert::AreEqual(Sulfur::Real(0.0), v.GetX());
      Assert::AreEqual(Sulfur::Real(0.0), v.GetY());
    }

    TEST_METHOD(Vector2Set2)
    {
      Sulfur::Vector2 v;

      v.SetX(0.5);
      v.SetY(-0.5);

      Assert::AreEqual(Sulfur::Real(0.5), v[0]);
      Assert::AreEqual(Sulfur::Real(-0.5), v[1]);
    }

    TEST_METHOD(Vector2Set3)
    {
      Sulfur::Vector2 v;

      v.Set(-2, -10);

      Assert::AreEqual(Sulfur::Real(-2.0), v[0]);
      Assert::AreEqual(Sulfur::Real(-10), v[1]);
    }

    TEST_METHOD(Vector2Dot0)
    {
      Sulfur::Vector2 v1(-5.0, 10.0);
      Sulfur::Vector2 v2(1.0, 2.0);

      Assert::AreEqual(Sulfur::Real(15.0), v1.Dot(v2));
      Assert::AreEqual(Sulfur::Real(15.0), Sulfur::Dot(v1, v2));
    }

    TEST_METHOD(Vector2Dot1)
    {
      Sulfur::Vector2 v1(0.002, 4.324234);
      Sulfur::Vector2 v2(-6.13123, 8.23421);

      Assert::IsTrue(AreEqual(Sulfur::Real(35.5943883851), v1.Dot(v2)));
      Assert::IsTrue(AreEqual(Sulfur::Real(35.5943883851), Sulfur::Dot(v1, v2)));
    }

    TEST_METHOD(Vector2Length0)
    {
      Sulfur::Vector2 v(3.645, -1.435);

      Assert::IsTrue(AreEqual((Sulfur::Real)std::sqrt(15.34525), v.Length()));
      Assert::IsTrue(AreEqual((Sulfur::Real)std::sqrt(15.34525), Sulfur::Length(v)));
      Assert::IsTrue(AreEqual(Sulfur::Real(15.34525), v.LengthSq()));
      Assert::IsTrue(AreEqual(Sulfur::Real(15.34525), Sulfur::LengthSq(v)));
    }

    TEST_METHOD(Vector2Normalize)
    {
      Sulfur::Vector2 v(4.325, 2.1666);
      Sulfur::Vector2 vn = v.Normalized();
      v.Normalize();

      Assert::IsTrue(AreEqual(Sulfur::Real(0.894087904119), vn[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.44789152672), vn[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.894087904119), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.44789152672), v[1]));

      v.Set(4.325, 2.1666);
      Sulfur::Normalize(v);

      Assert::IsTrue(AreEqual(Sulfur::Real(0.894087904119), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.44789152672), v[1]));
    }

    TEST_METHOD(Vector2ZeroOut)
    {
      Sulfur::Vector2 v(3.0, 9.4);

      v.ZeroOut();

      Assert::AreEqual(Sulfur::Real(0.0), v[0]);
      Assert::AreEqual(Sulfur::Real(0.0), v[1]);
    }

    TEST_METHOD(Vector2IsZero)
    {
      Sulfur::Vector2 v(0.0, 0.0);

      Assert::IsTrue(v.IsZero());
    }

    TEST_METHOD(Vector2IsZeroEpsilon)
    {
      Sulfur::Vector2 v(0.0000000001, 0.0000000001);

      Assert::IsTrue(v.IsZeroEpsilon());
    }

    TEST_METHOD(Vector2HasInfinity)
    {
      Sulfur::Vector2 v(std::numeric_limits<Sulfur::Real>::infinity(), 
        std::numeric_limits<Sulfur::Real>::infinity());

      Assert::IsTrue(v.HasInfinity());

      v.Set(0.0, std::numeric_limits<Sulfur::Real>::infinity());
      Assert::IsTrue(v.HasInfinity());

      v.Set(std::numeric_limits<Sulfur::Real>::infinity(), 0.0);
      Assert::IsTrue(v.HasInfinity());
    }

    TEST_METHOD(Vector2Abs)
    {
      Sulfur::Vector2 v(-1.0, -5.33);
      Sulfur::Vector2 va = v.GetAbs();
      v.Abs();

      Assert::AreEqual(Sulfur::Real(1.0), va[0]);
      Assert::AreEqual(Sulfur::Real(5.33), va[1]);
      Assert::AreEqual(Sulfur::Real(1.0), v[0]);
      Assert::AreEqual(Sulfur::Real(5.33), v[1]);

      v.Set(2.4432, 0.442131);
      v.Abs();
      Assert::AreEqual(Sulfur::Real(2.4432), v[0]);
      Assert::AreEqual(Sulfur::Real(0.442131), v[1]);
    }

    TEST_METHOD(Vector2Negate)
    {
      Sulfur::Vector2 v(2.423, -0.98775);
      Sulfur::Vector2 vn = v.Negated();
      v.Negate();

      Assert::AreEqual(Sulfur::Real(-2.423), vn[0]);
      Assert::AreEqual(Sulfur::Real(0.98775), vn[1]);
      Assert::AreEqual(Sulfur::Real(-2.423), v[0]);
      Assert::AreEqual(Sulfur::Real(0.98775), v[1]);

      Sulfur::Negate(v);
      Assert::AreEqual(Sulfur::Real(2.423), v[0]);
      Assert::AreEqual(Sulfur::Real(-0.98775), v[1]);
    }

    TEST_METHOD(Vector2Distance)
    {
      Sulfur::Vector2 v1(-1.2345, 3.52348);
      Sulfur::Vector2 v2(6.25219, -9.24234);

      Assert::IsTrue(AreEqual(Sulfur::Real(219.0166874285), v1.DistanceSq(v2)));
      Assert::IsTrue(AreEqual(Sulfur::Real(219.0166874285), Sulfur::DistanceSq(v1, v2)));
      Assert::IsTrue(AreEqual(Sulfur::Real(14.799212392168), v1.Distance(v2)));
      Assert::IsTrue(AreEqual(Sulfur::Real(14.799212392168), Sulfur::Distance(v1, v2)));
    }

    TEST_METHOD(Vector2MinMaxAxis)
    {
      Sulfur::Vector2 v(0.0, 2.0);

      Assert::AreEqual(1, v.MaxAxis());
      Assert::AreEqual(0, v.MinAxis());
      Assert::AreEqual(Sulfur::Real(2.0), v.MaxAxisValue());
      Assert::AreEqual(Sulfur::Real(0.0), v.MinAxisValue());

      v.Set(2.0, -3.14);
      Assert::AreEqual(0, v.MaxAxis());
      Assert::AreEqual(1, v.MinAxis());
      Assert::AreEqual(Sulfur::Real(2.0), v.MaxAxisValue());
      Assert::AreEqual(Sulfur::Real(-3.14), v.MinAxisValue());
    }

    TEST_METHOD(Vector2Lerp)
    {
      Sulfur::Vector2 v1(-1.0, 0.5);
      Sulfur::Vector2 v2(6.0, 4.5);
      Sulfur::Vector2 lerped = v1.Lerp(v2, 0.5);

      Assert::AreEqual(Sulfur::Real(2.5), lerped[0]);
      Assert::AreEqual(Sulfur::Real(2.5), lerped[1]);
    }

    TEST_METHOD(Vector2Rotate)
    {
      Sulfur::Vector2 v(1.0, 0.0);
      Sulfur::Vector2 vr = v.RotatedDeg(90);
      v.RotateDeg(90);

      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), vr[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), vr[1]));

      v.Set(0.0, -1.0);
      vr = v.RotatedRad(SF_PI);
      v.RotateRad(SF_PI);

      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), v[1]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), vr[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), vr[1]));
    }

    TEST_METHOD(Vector2Angle)
    {
      Sulfur::Vector2 v1(-1.0, 0.0);
      Sulfur::Vector2 v2(1.0, 0.0);

      Assert::IsTrue(AreEqual(SF_PI, v1.GetAngle(v2)));
    }

    TEST_METHOD(Vector2OpAdd)
    {
      Sulfur::Vector2 v1(2.4, 0.3);
      Sulfur::Vector2 v2(1.6, 2.7);
      Sulfur::Vector2 sum = v1 + v2;

      Assert::IsTrue(AreEqual(Sulfur::Real(4.0), sum[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.0), sum[1]));

      v1.Set(0.0, 0.0);
      v2.Set(0.0, 0.0);
      sum = v1 + v2;
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), sum[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), sum[1]));

      v1.Set(0.312, 1.678);
      v2 += v1;
      Assert::IsTrue(AreEqual(Sulfur::Real(0.312), v2[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(1.678), v2[1]));
    }

    TEST_METHOD(Vector2OpNeg)
    {
      Sulfur::Vector2 v(1.0, -2.0);
      Sulfur::Vector2 vn = -v;

      Assert::IsTrue(AreEqual(Sulfur::Real(-1.0), vn[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(2.0), vn[1]));

      vn = -vn;
      Assert::IsTrue(AreEqual(Sulfur::Real(1.0), vn[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(-2.0), vn[1]));
    }

    TEST_METHOD(Vector2OpSub)
    {
      Sulfur::Vector2 v1(5.5, 4.5);
      Sulfur::Vector2 v2(0.5, 0.5);
      Sulfur::Vector2 res = v1 - v2;

      Assert::IsTrue(AreEqual(Sulfur::Real(5.0), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(4.0), res[1]));

      v1.Set(2.0, 15.0);
      v2.Set(2.0, 15.0);
      v1 -= v2;
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), v1[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(0.0), v1[1]));
    }

    TEST_METHOD(Vector2OpMul)
    {
      Sulfur::Vector2 v(2.5, 3.5);
      Sulfur::Real s = 2.0;
      Sulfur::Vector2 res = 2 * v;

      Assert::IsTrue(AreEqual(Sulfur::Real(5.0), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(7.0), res[1]));

      v *= s;

      Assert::IsTrue(AreEqual(Sulfur::Real(5.0), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(7.0), v[1]));
    }

    TEST_METHOD(Vector2OpDiv)
    {
      Sulfur::Vector2 v(5.0, 7.0);
      Sulfur::Real s = 2.0;
      Sulfur::Vector2 res = v / s;

      Assert::IsTrue(AreEqual(Sulfur::Real(2.5), res[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.5), res[1]));

      v /= s;

      Assert::IsTrue(AreEqual(Sulfur::Real(2.5), v[0]));
      Assert::IsTrue(AreEqual(Sulfur::Real(3.5), v[1]));
    }

    TEST_METHOD(Vector2OpEq)
    {
      Sulfur::Vector2 v1(2.51234, 9.1234);
      Sulfur::Vector2 v2(2.51234, 9.1234);
      Sulfur::Vector2 v3(0.0, 0.0);

      Assert::IsTrue(v1 == v2);
      Assert::IsTrue(v1 != v3);
    }
	};
}

#pragma warning(default : 4305)