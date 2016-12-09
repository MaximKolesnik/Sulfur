/******************************************************************************/
/*!
\par     Sulfur
\file    utObject.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"

#include <stack>
#include <algorithm>

#include "Types/sfTypes.hpp"

#include "DataStructures/sfSlotMap.hpp"
#include "Components/sfTransform.hpp"

#include "Types\sfObject.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Factories\sfComponentFactory.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

//only for test 4
bool TestChildren(const Object::ChildrenSet &children)
{
  bool res = true;

  if (!children.empty() && children.size() != 2)
    return false;

  for (auto it : children)
  {
    if (!SF_GET_OBJECT(it)->HasComponent<Transform>())
      return false;
    
    res = res && TestChildren(SF_GET_OBJECT(it)->GetChildren());
  }

  return res;
}

namespace UnitTests
{
  TEST_CLASS(utObject)
  {
    TEST_METHOD(Object1)
    {
      ComponentFactory::Instance()->Initialize();

      Object *parent = SF_CREATE_OBJECT("Test");
      Object *child1 = SF_CREATE_OBJECT("Test");
      Object *child2 = SF_CREATE_OBJECT("Test");
      Object *grandChild11 = SF_CREATE_OBJECT("Test");
      Object *grandChild12 = SF_CREATE_OBJECT("Test");
      Object *grandChild21 = SF_CREATE_OBJECT("Test");
      Object *grandChild22 = SF_CREATE_OBJECT("Test");

      HNDL parentHndl = parent->GetHndl();
      HNDL child1Hndl = child1->GetHndl();
      HNDL child2Hndl = child2->GetHndl();
      HNDL grandChild11Hndl = grandChild11->GetHndl();
      HNDL grandChild12Hndl = grandChild12->GetHndl();
      HNDL grandChild21Hndl = grandChild21->GetHndl();
      HNDL grandChild22Hndl = grandChild22->GetHndl();

      child1->SetParent(parentHndl);
      child2->SetParent(parentHndl);
      grandChild11->SetParent(child1Hndl);
      grandChild12->SetParent(child1Hndl);
      grandChild21->SetParent(child2Hndl);
      grandChild22->SetParent(child2Hndl);

      Assert::IsTrue(parent->HasDescendant(child1Hndl));
      Assert::IsTrue(parent->HasDescendant(child2Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild11Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild12Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild21Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild22Hndl));

      Assert::IsTrue(child1->HasDescendant(grandChild11Hndl));
      Assert::IsTrue(child1->HasDescendant(grandChild12Hndl));
      Assert::IsTrue(child2->HasDescendant(grandChild21Hndl));
      Assert::IsTrue(child2->HasDescendant(grandChild22Hndl));

      Assert::IsFalse(child1->HasDescendant(parentHndl));
      Assert::IsFalse(child2->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild11->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild12->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild21->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild22->HasDescendant(parentHndl));

      ObjectFactory::Instance()->DestroyObject(grandChild22Hndl);
      ObjectFactory::Instance()->EndFrameCleanUp();

      Assert::IsNull(SF_GET_OBJECT(grandChild22Hndl));

      ObjectFactory::Instance()->DestroyObject(grandChild21Hndl);
      ObjectFactory::Instance()->DestroyObject(grandChild12Hndl);
      ObjectFactory::Instance()->DestroyObject(grandChild11Hndl);
      ObjectFactory::Instance()->DestroyObject(child2Hndl);
      ObjectFactory::Instance()->DestroyObject(child1Hndl);
      ObjectFactory::Instance()->DestroyObject(parentHndl);

      ObjectFactory::Instance()->EndFrameCleanUp();

      Assert::IsNull(SF_GET_OBJECT(grandChild22Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild21Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild12Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild11Hndl));
      Assert::IsNull(SF_GET_OBJECT(child2Hndl));
      Assert::IsNull(SF_GET_OBJECT(child1Hndl));
      Assert::IsNull(SF_GET_OBJECT(parentHndl));
    }

    TEST_METHOD(Object2)
    {
      ComponentFactory::Instance()->Initialize();

      Object *parent = SF_CREATE_OBJECT("Test");
      Object *child1 = SF_CREATE_OBJECT("Test");
      Object *child2 = SF_CREATE_OBJECT("Test");
      Object *grandChild11 = SF_CREATE_OBJECT("Test");
      Object *grandChild12 = SF_CREATE_OBJECT("Test");
      Object *grandChild21 = SF_CREATE_OBJECT("Test");
      Object *grandChild22 = SF_CREATE_OBJECT("Test");

      HNDL parentHndl = parent->GetHndl();
      HNDL child1Hndl = child1->GetHndl();
      HNDL child2Hndl = child2->GetHndl();
      HNDL grandChild11Hndl = grandChild11->GetHndl();
      HNDL grandChild12Hndl = grandChild12->GetHndl();
      HNDL grandChild21Hndl = grandChild21->GetHndl();
      HNDL grandChild22Hndl = grandChild22->GetHndl();

      child1->SetParent(parentHndl);
      child2->SetParent(parentHndl);
      grandChild11->SetParent(child1Hndl);
      grandChild12->SetParent(child1Hndl);
      grandChild21->SetParent(child2Hndl);
      grandChild22->SetParent(child2Hndl);

      ObjectFactory::Instance()->DestroyObject(parentHndl);
      ObjectFactory::Instance()->EndFrameCleanUp();

      Assert::IsNull(SF_GET_OBJECT(grandChild22Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild21Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild12Hndl));
      Assert::IsNull(SF_GET_OBJECT(grandChild11Hndl));
      Assert::IsNull(SF_GET_OBJECT(child2Hndl));
      Assert::IsNull(SF_GET_OBJECT(child1Hndl));
      Assert::IsNull(SF_GET_OBJECT(parentHndl));
    }

    TEST_METHOD(Object3)
    {
      ComponentFactory::Instance()->Initialize();

      Object *empty = SF_CREATE_EMPTY_OBJECT("TestEmpty");
      Object *obj = SF_CREATE_OBJECT("Test");

      Assert::IsFalse(empty->HasComponent("Transform"));
      Assert::IsFalse(empty->HasComponent<Transform>());

      Assert::IsTrue(obj->HasComponent("Transform"));
      Assert::IsTrue(obj->HasComponent<Transform>());

      Assert::IsNull(empty->GetComponent("Transform"));
      Assert::IsNull(empty->GetComponent<Transform>());

      Assert::IsNotNull(obj->GetComponent("Transform"));
      Assert::IsNotNull(obj->GetComponent<Transform>());

      Object *clone = obj->Clone();
      Assert::IsNotNull(clone->GetComponent("Transform"));
      Assert::IsNotNull(clone->GetComponent<Transform>());
      Assert::IsTrue(clone->HasComponent("Transform"));
      Assert::IsTrue(clone->HasComponent<Transform>());
    }

    TEST_METHOD(Object4)
    {
      ComponentFactory::Instance()->Initialize();

      Object *parent = SF_CREATE_OBJECT("Test");
      Object *child1 = SF_CREATE_OBJECT("Test");
      Object *child2 = SF_CREATE_OBJECT("Test");
      Object *grandChild11 = SF_CREATE_OBJECT("Test");
      Object *grandChild12 = SF_CREATE_OBJECT("Test");
      Object *grandChild21 = SF_CREATE_OBJECT("Test");
      Object *grandChild22 = SF_CREATE_OBJECT("Test");

      HNDL parentHndl = parent->GetHndl();
      HNDL child1Hndl = child1->GetHndl();
      HNDL child2Hndl = child2->GetHndl();
      HNDL grandChild11Hndl = grandChild11->GetHndl();
      HNDL grandChild12Hndl = grandChild12->GetHndl();
      HNDL grandChild21Hndl = grandChild21->GetHndl();
      HNDL grandChild22Hndl = grandChild22->GetHndl();

      child1->SetParent(parentHndl);
      child2->SetParent(parentHndl);
      grandChild11->SetParent(child1Hndl);
      grandChild12->SetParent(child1Hndl);
      grandChild21->SetParent(child2Hndl);
      grandChild22->SetParent(child2Hndl);

      Assert::IsTrue(parent->HasDescendant(child1Hndl));
      Assert::IsTrue(parent->HasDescendant(child2Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild11Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild12Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild21Hndl));
      Assert::IsTrue(parent->HasDescendant(grandChild22Hndl));

      Assert::IsTrue(child1->HasDescendant(grandChild11Hndl));
      Assert::IsTrue(child1->HasDescendant(grandChild12Hndl));
      Assert::IsTrue(child2->HasDescendant(grandChild21Hndl));
      Assert::IsTrue(child2->HasDescendant(grandChild22Hndl));

      Assert::IsFalse(child1->HasDescendant(parentHndl));
      Assert::IsFalse(child2->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild11->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild12->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild21->HasDescendant(parentHndl));
      Assert::IsFalse(grandChild22->HasDescendant(parentHndl));

      Object *clone = parent->Clone();

      auto cloneChildren = clone->GetChildren();

      Assert::IsTrue(TestChildren(cloneChildren));
    }
  };
}