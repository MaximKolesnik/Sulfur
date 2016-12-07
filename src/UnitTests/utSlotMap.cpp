/******************************************************************************/
/*!
\par     Sulfur
\file    utSlotMap.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    10/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"

#include <stack>
#include <algorithm>
#include <set>

#include "Types/sfTypes.hpp"

#include "DataStructures/sfSlotMap.hpp"
#include "Components/sfTransform.hpp"

#include "Factories\sfObjectFactory.hpp"
#include "Factories\sfComponentFactory.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;


namespace UnitTests
{
  TEST_CLASS(utSlotMap)
  {
    TEST_METHOD(SlotMap1)
    {
      std::vector<HNDL> allocations;
      SlotMap<Transform> slotMap;

      for (Sulfur::UINT64 i = 0; i < EngineSettings::SlotMapObjsPerPage; ++i)
        allocations.push_back(slotMap.Create());

      for (Sulfur::UINT64 i = 0; i < EngineSettings::SlotMapObjsPerPage; ++i)
        Assert::IsTrue(allocations[i] == i);
    }

    TEST_METHOD(SlotMap2)
    {
      SlotMap<Transform> slotMap;
      slotMap.CreateAt(0);
      slotMap.Erase(0);
      slotMap.CreateAt(0);
    }

    TEST_METHOD(SlotMap3)
    {
      SlotMap<Transform> slotMap;

      Transform *t = slotMap.At(10);

      Assert::IsTrue(t == nullptr);
    }

    TEST_METHOD(SlotMap4)
    {
      SlotMap<Transform> slotMap;
      std::stack<HNDL> allocatedHndls;
      Sulfur::UINT64 numAllocs = 10 * EngineSettings::SlotMapObjsPerPage;

      for (Sulfur::UINT64 i = 0; i < numAllocs; ++i)
        allocatedHndls.push(slotMap.Create());

      while (!allocatedHndls.empty())
      {
        slotMap.Erase(allocatedHndls.top());
        allocatedHndls.pop();
      }
    }

    TEST_METHOD(SlotMap5)
    {
      SlotMap<Transform> slotMap;
      std::vector<HNDL> hndls;
      Sulfur::UINT64 numAllocs = 100 * EngineSettings::SlotMapObjsPerPage;

      for (Sulfur::UINT64 i = 0; i < numAllocs; ++i)
        hndls.push_back(i);

      std::random_shuffle(hndls.begin(), hndls.end());

      for (auto &it : hndls)
        slotMap.CreateAt(it);

      std::random_shuffle(hndls.begin(), hndls.end());

      while (!hndls.empty())
      {
        slotMap.Erase(hndls.back());
        hndls.pop_back();
      }

      Assert::IsTrue(slotMap.GetNumberOfAllocPages() == 3);
    }

    TEST_METHOD(SlotMap6)
    {
      SlotMap<Transform> slotMap;
      std::vector<HNDL> hndls;
      Sulfur::UINT64 numAllocs = 10000 * EngineSettings::SlotMapObjsPerPage;

      for (Sulfur::UINT64 i = 0; i < numAllocs; ++i)
        hndls.push_back(i);

      std::random_shuffle(hndls.begin(), hndls.end());

      for (auto &it : hndls)
        slotMap.CreateAt(it);

      std::random_shuffle(hndls.begin(), hndls.end());

      while (!hndls.empty())
      {
        slotMap.Erase(hndls.back());
        hndls.pop_back();
      }

      Assert::IsTrue(slotMap.GetNumberOfAllocPages() == 3);
    }

    //Take too long, but it passes
    /*TEST_METHOD(SlotMap7)
    {
      SlotMap<Transform> slotMap;
      std::vector<HNDL> hndls;
      Sulfur::UINT64 numAllocs = 100000 * EngineSettings::SlotMapObjsPerPage;

      for (Sulfur::UINT64 i = 0; i < numAllocs; ++i)
        hndls.push_back(i);

      std::random_shuffle(hndls.begin(), hndls.end());

      for (auto &it : hndls)
        slotMap.CreateAt(it);

      std::random_shuffle(hndls.begin(), hndls.end());

      while (!hndls.empty())
      {
        slotMap.Erase(hndls.back());
        hndls.pop_back();
      }

      Assert::IsTrue(slotMap.GetNumberOfAllocPages() == 2);
    }*/

    TEST_METHOD(SlotMap8)
    {
      SlotMap<Transform> slotMap;
      std::vector<HNDL> hndls;
      Sulfur::UINT64 numAllocs = 1000 * EngineSettings::SlotMapObjsPerPage;

      for (Sulfur::UINT64 i = 0; i < numAllocs; ++i)
        hndls.push_back(i);

      std::random_shuffle(hndls.begin(), hndls.end());

      for (auto &it : hndls)
        slotMap.CreateAt(it);

      slotMap.Clear();

      Assert::IsTrue(slotMap.GetNumberOfAllocPages() == 3);

      for (auto &it : hndls)
        slotMap.CreateAt(it);

      std::random_shuffle(hndls.begin(), hndls.end());
      for (auto &it : hndls)
        slotMap.Erase(it);
    }

    TEST_METHOD(SlotMap9)
    {
      ComponentFactory::Instance()->Initialize();
      std::vector<HNDL> createdTransforms;
      std::vector<HNDL> iteratedTransforms;
      for (int i = 0; i < 20000; ++i)
      {
        Object *obj = SF_CREATE_OBJECT("Default");
        createdTransforms.push_back(obj->GetComponentHandle<Transform>());
      }

      auto compData = SF_GET_COMP_DATA(Transform);

      for (auto it = compData.begin(); it != compData.end(); ++it)
        iteratedTransforms.push_back((*it)->GetHndl());

      Assert::IsTrue(createdTransforms.size() == iteratedTransforms.size());

      for (auto it : iteratedTransforms)
      {
        auto res = std::find(createdTransforms.begin(), createdTransforms.end(), it);
        Assert::IsTrue(res != createdTransforms.end());
      }
    }
  };
}