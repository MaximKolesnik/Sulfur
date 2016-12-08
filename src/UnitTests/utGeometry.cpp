/******************************************************************************/
/*!
\par     Sulfur
\file    utGeometry.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    12/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "Math\Geometry\sfGeometry.hpp"
#include "Math\Geometry\sfShapes.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfQuaternion.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;
using namespace Sulfur::Geometry;

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

  static const Vector3 p0 = Vector3(1, 0, 0);
  static const Vector3 p1 = Vector3(1, 1, 0);
  static const Vector3 p2 = Vector3(1, 0, 1);

  TEST_CLASS(utGeometry)
  {
  public:
    TEST_METHOD(PlaneTransform)
    {
      Vector3 tp0, tp1, tp2, normal, tNormal;
      Real d, td;
      Matrix4 trans;
      Quaternion rot;
      Plane plane, tPlane;

      rot.SetEuler(SF_PI / 2, 0, 0);
      trans.SetTransformation(rot, Vector3(1, 1, 1), Vector3(0,0,0));

      plane.Set(p0, p1, p2);
      plane.Transform(trans);

      tp0 = trans * p0;
      tp1 = trans * p1;
      tp2 = trans * p2;

      tPlane.Set(tp0, tp1, tp2);

      normal = plane.GetNormal();
      d = plane.GetDistance();
      tNormal = tPlane.GetNormal();
      td = tPlane.GetDistance();

      Assert::IsTrue(AreEqual(normal[0], tNormal[0]));
      Assert::IsTrue(AreEqual(normal[1], tNormal[1]));
      Assert::IsTrue(AreEqual(normal[2], tNormal[2]));
      Assert::IsTrue(AreEqual(d, td));
      
      rot.SetEuler(SF_PI / 4, 0, 0);
      trans.SetTransformation(rot, Vector3(2, 1, 2), Vector3(5, 0, -7));

      plane.Set(p0, p1, p2);
      plane.Transform(trans);

      tp0 = trans * p0;
      tp1 = trans * p1;
      tp2 = trans * p2;

      tPlane.Set(tp0, tp1, tp2);

      normal = plane.GetNormal();
      d = plane.GetDistance();
      tNormal = tPlane.GetNormal();
      td = tPlane.GetDistance();

      Assert::IsTrue(AreEqual(normal[0], tNormal[0]));
      Assert::IsTrue(AreEqual(normal[1], tNormal[1]));
      Assert::IsTrue(AreEqual(normal[2], tNormal[2]));
      Assert::IsTrue(AreEqual(d, td));


      rot.SetEuler(SF_PI / 4, 0, SF_PI / 5.6);
      trans.SetTransformation(rot, Vector3(2, 5, 2), Vector3(5, 10, -7));

      plane.Set(p0, p1, p2);
      plane.Transform(trans);

      tp0 = trans * p0;
      tp1 = trans * p1;
      tp2 = trans * p2;

      tPlane.Set(tp0, tp1, tp2);

      normal = plane.GetNormal();
      d = plane.GetDistance();
      tNormal = tPlane.GetNormal();
      td = tPlane.GetDistance();

      Assert::IsTrue(AreEqual(normal[0], tNormal[0]));
      Assert::IsTrue(AreEqual(normal[1], tNormal[1]));
      Assert::IsTrue(AreEqual(normal[2], tNormal[2]));
      Assert::IsTrue(AreEqual(d, td));

      rot.SetEuler(0, SF_PI / 3.2, -SF_PI / 5.6);
      trans.SetTransformation(rot, Vector3(2, 5, 2), Vector3(5, 10, -7));

      plane.Set(p0, p1, p2);
      plane.Transform(trans);

      tp0 = trans * p0;
      tp1 = trans * p1;
      tp2 = trans * p2;

      tPlane.Set(tp0, tp1, tp2);

      normal = plane.GetNormal();
      d = plane.GetDistance();
      tNormal = tPlane.GetNormal();
      td = tPlane.GetDistance();

      Assert::IsTrue(AreEqual(normal[0], tNormal[0]));
      Assert::IsTrue(AreEqual(normal[1], tNormal[1]));
      Assert::IsTrue(AreEqual(normal[2], tNormal[2]));
      Assert::IsTrue(AreEqual(d, td));
    }
  };
}