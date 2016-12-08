/******************************************************************************/
/*!
\par     Sulfur
\file    utMinMaxHeap.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    10/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <algorithm>
#include <functional>

#include "Types/sfTypes.hpp"

#include "DataStructures/sfMinMaxHeap.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

namespace UnitTests
{
  TEST_CLASS(utMinMaxHeap)
  {
    TEST_METHOD(MinMaxHeap1)
    {
      MinMaxHeap<int> heap;
      std::vector<int> vals;

      for (int i = 0; i < 100; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 100; ++i)
        heap.Insert(vals[i]);

      Assert::IsTrue(heap.GetMin() == 0);
      Assert::IsTrue(heap.GetMax() == 99);
      Assert::IsTrue(heap.IsMinMaxHeap());
    }

    TEST_METHOD(MinMaxHeap2)
    {
      MinMaxHeap<int> heap;
      std::vector<int> vals;

      for (int i = 0; i < 5000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 5000; ++i)
        heap.Insert(vals[i]);

      Assert::IsTrue(heap.GetMin() == 0);
      Assert::IsTrue(heap.GetMax() == 4999);
      Assert::IsTrue(heap.IsMinMaxHeap());
    }

    TEST_METHOD(MinMaxHeap3)
    {
      MinMaxHeap<int> heap;
      std::vector<int> vals;

      for (int i = 0; i < 5000; ++i)
        vals.push_back(i);

      std::vector<int> shuffledVals = vals;
      std::random_shuffle(shuffledVals.begin(), shuffledVals.end());

      for (int i = 0; i < 5000; ++i)
        heap.Insert(shuffledVals[i]);

      for (int i = 0; i < 5000; ++i)
      {
        int maxEl = heap.GetMax();
        Assert::IsTrue(maxEl == vals.back());
        vals.pop_back();
        heap.ExtractMax();
        Assert::IsTrue(heap.IsMinMaxHeap());
      }
      Assert::IsTrue(heap.IsMinMaxHeap());
    }

    TEST_METHOD(MinMaxHeap4)
    {
      MinMaxHeap<int> heap;
      std::vector<int> vals;

      for (int i = 0; i < 5000; ++i)
        vals.push_back(i);

      std::vector<int> shuffledVals = vals;
      std::random_shuffle(shuffledVals.begin(), shuffledVals.end());

      for (int i = 0; i < 5000; ++i)
        heap.Insert(shuffledVals[i]);

      std::reverse(vals.begin(), vals.end());

      for (int i = 0; i < 5000; ++i)
      {
        int minEl = heap.GetMin();
        Assert::IsTrue(minEl == vals.back());
        vals.pop_back();
        heap.ExtractMin();
        Assert::IsTrue(heap.IsMinMaxHeap());
      }
      Assert::IsTrue(heap.IsMinMaxHeap());
    }

    TEST_METHOD(MinMaxHeap5)
    {
      MinMaxHeap<int> heap, heap2;
      std::vector<int> vals;

      for (int i = 0; i < 2000; ++i)
        vals.push_back(i);

      std::vector<int> shuffledVals = vals;
      std::random_shuffle(shuffledVals.begin(), shuffledVals.end());

      for (int i = 0; i < 2000; ++i)
        heap.Insert(shuffledVals[i]);

      std::reverse(vals.begin(), vals.end());

      heap2 = heap;

      for (int i = 0; i < 2000; ++i)
      {
        int minEl = heap2.GetMin();
        Assert::IsTrue(minEl == vals.back());
        vals.pop_back();
        heap2.ExtractMin();
        Assert::IsTrue(heap2.IsMinMaxHeap());
      }
      Assert::IsTrue(heap2.IsMinMaxHeap());
    }

    TEST_METHOD(MinMaxHeap6)
    {
      MinMaxHeap<int> heap;
      std::vector<int> vals;

      for (int i = 0; i < 2000; ++i)
        vals.push_back(i);

      std::vector<int> shuffledVals = vals;
      std::random_shuffle(shuffledVals.begin(), shuffledVals.end());

      for (int i = 0; i < 2000; ++i)
        heap.Insert(shuffledVals[i]);

      std::reverse(vals.begin(), vals.end());

      MinMaxHeap<int> heap2(heap);

      for (int i = 0; i < 2000; ++i)
      {
        int minEl = heap2.GetMin();
        Assert::IsTrue(minEl == vals.back());
        vals.pop_back();
        heap2.ExtractMin();
        Assert::IsTrue(heap2.IsMinMaxHeap());
      }
      Assert::IsTrue(heap2.IsMinMaxHeap());
    }
  };
}