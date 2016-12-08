/******************************************************************************/
/*!
\par     Sulfur
\file    utBST.cpp
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

#include "DataStructures/sfBST.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

static const BST<int>::Node *prev = nullptr;

bool IsBST(const BST<int>::Node *node)
{
  //static const BST<int>::Node *prev = nullptr;

  if (node)
  {
    if (!IsBST(node->m_left))
      return false;

    if (prev != nullptr && node->m_data <= prev->m_data)
      return false;

    prev = node;

    return IsBST(node->m_right);
  }

  return true;
}

namespace UnitTests
{
  TEST_CLASS(utBST)
  {
    TEST_METHOD(BST1)
    {
      prev = nullptr;
      BST<int> bst;

      for (int i = 0; i < 100; ++i)
        Assert::IsTrue(bst.Insert(i));

      Assert::IsTrue(bst.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(bst.Size() == 100);
    }

    TEST_METHOD(BST2)
    {
      prev = nullptr;

      BST<int> bst;
      std::vector<int> vals;

      for (int i = 0; i < 1000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        Assert::IsTrue(bst.Insert(it));

      std::random_shuffle(vals.begin(), vals.end());
      for (int i = 0; i < 200; ++i)
        Assert::IsTrue(bst.Remove(vals[i]));

      Assert::IsTrue(bst.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(bst.Size() == 800);
    }

    TEST_METHOD(BST3)
    {
      prev = nullptr;

      BST<int> bst;
      std::vector<int> vals;

      for (int i = 0; i < 5000; ++i)
        vals.push_back(i);
      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        Assert::IsTrue(bst.Insert(it));

      std::random_shuffle(vals.begin(), vals.end());

      for (auto it : vals)
        Assert::IsFalse(bst.Insert(it));

      vals.clear();

      for (int i = 5000; i < 10000; ++i)
        vals.push_back(i);

      for (auto it : vals)
        Assert::IsFalse(bst.Remove(it));

      Assert::IsTrue(bst.Size() == 5000);
    }

    TEST_METHOD(BST4)
    {
      prev = nullptr;

      BST<int> bst;
      std::vector<int> vals;

      for (int i = 0; i < 25000; ++i)
        vals.push_back(i);

      for (auto it : vals)
        Assert::IsTrue(bst.Insert(it));

      Assert::IsTrue(bst.Size() == 25000);
      Assert::IsTrue(bst.RunCallBackOnRoot(IsBST));
      prev = nullptr;

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 22000; ++i)
        Assert::IsTrue(bst.Remove(vals[i]));

      for (int i = 22000; i < 25000; ++i)
        Assert::IsTrue(bst.Find(vals[i]));

      for (int i = 0; i < 5000; ++i)
        Assert::IsFalse(bst.Find(vals[i]));

      Assert::IsTrue(bst.Size() == 3000);
      Assert::IsTrue(bst.RunCallBackOnRoot(IsBST));
      prev = nullptr;

      bst.Clear();
      Assert::IsTrue(bst.Size() == 0);
    }
  };
}