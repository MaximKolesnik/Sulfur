#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include <algorithm>

#include "DataStructures/sfAVL.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

static const AVL<int>::Node *prev = nullptr;

Sulfur::INT64 Height(const AVL<int>::Node *node)
{
  if (!node)
    return 0;

  Sulfur::INT64 lh = Height(node->m_left);
  Sulfur::INT64 rh = Height(node->m_right);

  return 1 + ((lh >= rh) ? lh : rh);
}

bool IsBalanced(const AVL<int>::Node *node)
{
  Sulfur::INT64 leftHeight, rightHeight;

  if (!node)
    return true;

  leftHeight = Height(node->m_left);
  rightHeight = Height(node->m_right);

  if (std::abs(leftHeight - rightHeight) > 1)
    leftHeight = leftHeight;

  if (std::abs(leftHeight - rightHeight) <= 1
      &&
      IsBalanced(node->m_left) && IsBalanced(node->m_right))
    return true;

  return false;
}

bool IsBST(const AVL<int>::Node *node)
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
  TEST_CLASS(utAVL)
  {
    TEST_METHOD(AVL1)
    {
      AVL<int> avl;

      for (int i = 0; i < 500; ++i)
        Assert::IsTrue(avl.Insert(i));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 500);

      avl.Clear();
      Assert::IsTrue(avl.Empty());
    }

    TEST_METHOD(AVL2)
    {
      AVL<int> avl;

      for (int i = 499; i >= 0; --i)
        Assert::IsTrue(avl.Insert(i));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 500);

      avl.Clear();
      Assert::IsTrue(avl.Empty());
    }

    TEST_METHOD(AVL3)
    {
      AVL<int> avl;

      avl.Insert(15);
      avl.Insert(14);
      avl.Insert(18);
      avl.Insert(19);
      avl.Insert(16);
      avl.Insert(17);

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;
    }

    TEST_METHOD(AVL4)
    {
      AVL<int> avl;
      std::vector<int> vals;

      for (int i = 0; i < 20; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        Assert::IsTrue(avl.Insert(it));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 20);
      Assert::IsFalse(avl.Empty());

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 5; ++i)
        Assert::IsTrue(avl.Remove(vals[i]));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 15);
      Assert::IsFalse(avl.Empty());

      for (int i = 10; i < 15; ++i)
        Assert::IsTrue(avl.Find(vals[i]));

      for (int i = 0; i < 5; ++i)
        Assert::IsFalse(avl.Find(vals[i]));
    }

    TEST_METHOD(AVL5)
    {
      AVL<int> avl;
      std::vector<int> vals;

      for (int i = 0; i < 200; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        Assert::IsTrue(avl.Insert(it));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 200);
      Assert::IsFalse(avl.Empty());

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 50; ++i)
        Assert::IsTrue(avl.Remove(vals[i]));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 150);
      Assert::IsFalse(avl.Empty());

      for (int i = 100; i < 200; ++i)
        Assert::IsTrue(avl.Find(vals[i]));

      for (int i = 0; i < 50; ++i)
        Assert::IsFalse(avl.Find(vals[i]));
    }

    TEST_METHOD(AVL6)
    {
      AVL<int> avl;
      std::vector<int> vals;

      for (int i = 0; i < 500; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        Assert::IsTrue(avl.Insert(it));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 500);
      Assert::IsFalse(avl.Empty());

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 490; ++i)
        Assert::IsTrue(avl.Remove(vals[i]));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;
    }

    TEST_METHOD(AVL7)
    {
      AVL<int> avl;
      std::vector<int> vals;

      for (int i = 0; i < 5000; ++i)
        vals.push_back(i);

      std::random_shuffle(vals.begin(), vals.end());

      for (auto &it : vals)
        Assert::IsTrue(avl.Insert(it));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 5000);
      Assert::IsFalse(avl.Empty());

      std::random_shuffle(vals.begin(), vals.end());

      for (int i = 0; i < 1000; ++i)
        Assert::IsTrue(avl.Remove(vals[i]));

      Assert::IsTrue(avl.RunCallBackOnRoot(IsBST));
      Assert::IsTrue(avl.RunCallBackOnRoot(IsBalanced));
      prev = nullptr;

      Assert::IsTrue(avl.Size() == 4000);
      Assert::IsFalse(avl.Empty());
    }
  };
}