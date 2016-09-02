#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <algorithm>
#include <functional>

#include "../src/Engine/Types/sfTypes.hpp"

#include "../src/Engine/DataStructures/sfHeap.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

namespace UnitTests
{
  TEST_CLASS(utHeap)
  {
    TEST_METHOD(Heap1)
    {
      Heap<int> heap;
      std::vector<int> vals;
      
      for (int i = 0; i < 20; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        heap.Insert(it);

      Assert::IsTrue(heap.GetRoot() == 0);

      heap.ExtractRoot();

      Assert::IsTrue(heap.GetRoot() == 1);
      Assert::IsTrue(heap.IsHeap());
    }

    TEST_METHOD(Heap2)
    {
      Heap<int> heap(3000);
      std::vector<int> vals;

      for (int i = 0; i < 3000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        heap.Insert(it);

      Assert::IsTrue(heap.GetRoot() == 0);
      Assert::IsTrue(heap.IsHeap());

      for (int i = 0; i < 1000; ++i)
      {
        int r = heap.ExtractRoot();
        Assert::IsTrue(r == i);
        Assert::IsTrue(heap.IsHeap());
      }
    }

    TEST_METHOD(Heap3)
    {
      Heap<int> heap(3000);
      std::vector<int> vals;

      for (int i = 0; i < 3000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        heap.Insert(it);

      Assert::IsTrue(heap.GetRoot() == 0);
      Assert::IsTrue(heap.IsHeap());

      std::vector<int> delInds;

      for (int i = 500; i < 1000; ++i)
        delInds.push_back(i);

      std::random_shuffle(delInds.begin(), delInds.end());

      for (auto it : delInds)
      {
        heap.Delete(it);
        Assert::IsTrue(heap.IsHeap());
      }

      Assert::IsTrue(heap.IsHeap());
    }

    TEST_METHOD(Heap4)
    {
      Heap<int, std::greater<int> > heap(3000);
      std::vector<int> vals;

      for (int i = 0; i < 3000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        heap.Insert(it);

      Assert::IsTrue(heap.GetRoot() == 2999);
      Assert::IsTrue(heap.IsHeap());

      std::vector<int> delInds;

      for (int i = 500; i < 1000; ++i)
        delInds.push_back(i);

      std::random_shuffle(delInds.begin(), delInds.end());

      for (auto it : delInds)
      {
        heap.Delete(it);
        Assert::IsTrue(heap.IsHeap());
      }

      Assert::IsTrue(heap.IsHeap());
    }
  };
}