#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <algorithm>

#include "../src/Engine/DataStructures/sfRBT.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

bool IsRed(const RBT<int>::Node *node)
{
  return (node != nullptr) && (node->m_color == RBT<int>::Node::Red);
}

int IsRBT(const RBT<int>::Node *node)
{
  int lh, rh;

  if (node == nullptr)
  {
    return 1;
  }
  else
  {
    const RBT<int>::Node *ln = node->m_left;
    const RBT<int>::Node *rn = node->m_right;

    /* Consecutive red links */
    if (IsRed(node))
    {
      if (IsRed(ln) || IsRed(rn))
       return false;
    }

    lh = IsRBT(ln);
    rh = IsRBT(rn);

    /* Invalid binary search tree */
    if ((ln != nullptr && ln->m_data >= node->m_data) 
      || 
      (rn != nullptr && rn->m_data <= node->m_data))
      return false;

    /* Black height mismatch */
    if (lh != 0 && rh != 0 && lh != rh)
      return false;

    /* Only count black links */
    if (lh != 0 && rh != 0)
      return (IsRed(node) ? lh : lh + 1);
    else
      return 0;
  }
}

namespace UnitTests
{
  TEST_CLASS(utRBT)
  {
    TEST_METHOD(RBT1)
    {
      RBT<int> rbt;
      std::vector<int> vals;

      for (int i = 0; i < 100; ++i)
        vals.push_back(i);

      for (auto it : vals)
        Assert::IsTrue(rbt.Insert(it));

      Assert::IsTrue(IsRBT(rbt.GetRoot()) != 0);
    }

    TEST_METHOD(RBT2)
    {
      RBT<int> rbt;
      std::vector<int> vals;

      for (int i = 0; i < 1500; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        Assert::IsTrue(rbt.Insert(it));

      Assert::IsTrue(IsRBT(rbt.GetRoot()) != 0);
      Assert::IsTrue(rbt.Size() == 1500);

      for (auto it : vals)
        Assert::IsTrue(rbt.Find(it));

      Assert::IsFalse(rbt.Empty());
      
      RBT<int> rbt2 = rbt;
      Assert::IsTrue(IsRBT(rbt2.GetRoot()) != 0);
      Assert::IsTrue(rbt2.Size() == 1500);

      rbt2 = rbt;
      Assert::IsTrue(IsRBT(rbt2.GetRoot()) != 0);
      Assert::IsTrue(rbt2.Size() == 1500);

      rbt.Clear();

      Assert::IsTrue(rbt.Empty());
      Assert::IsTrue(rbt.Size() == 0);
      Assert::IsNull(rbt.GetRoot());
    }

    TEST_METHOD(RBT3)
    {
      RBT<int> rbt;
      std::vector<int> vals;

      for (int i = 0; i < 3000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        Assert::IsTrue(rbt.Insert(it));

      Assert::IsTrue(IsRBT(rbt.GetRoot()) != 0);
      Assert::IsTrue(rbt.Size() == 3000);

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 1500; ++i)
        Assert::IsTrue(rbt.Remove(vals[i]));

      Assert::IsTrue(IsRBT(rbt.GetRoot()) != 0);
      Assert::IsTrue(rbt.Size() == 1500);

      for (int i = 1500; i < 3000; ++i)
        Assert::IsTrue(rbt.Remove(vals[i]));

      Assert::IsTrue(rbt.Size() == 0);
    }
  };
}