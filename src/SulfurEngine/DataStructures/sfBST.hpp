/******************************************************************************/
/*!
\par     Sulfur
\file    sfBST.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    8/28/2016

\brief   Non self-balancing binary search tree

All content � 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <functional>

#include "../Types/sfTypes.hpp"

namespace Sulfur
{
  template <class DataType, class Comparator = std::less<DataType>,
            class EqualityComp = std::equal_to<DataType> >
  class BST
  {
  public:
    struct Node
    {
      DataType m_data;

      Node *m_left = nullptr;
      Node *m_right = nullptr;
    };
    typedef bool(*CallBackFunc)(const Node *data);

    BST(Comparator comparator = Comparator(), 
      EqualityComp equalityComp = EqualityComp());
    BST(const BST &other);
    ~BST(void);

    const BST& operator=(const BST &other);

    bool Insert(const DataType &val);
    bool Remove(const DataType &val);

    void Clear(void);

    bool Find(const DataType &val) const;
    bool Empty(void) const;
    UINT64 Size(void) const;

    const Node* GetRoot(void) const { return m_root; }

    void BreadthFirstTraverse(CallBackFunc func) const;
    void InOrderTraverse(CallBackFunc func) const;
    void PostOrderTraverse(CallBackFunc func) const;

    bool RunCallBackOnRoot(CallBackFunc func) const
    {
      return func(m_root);
    }
  private:
    Node* _CreateNode(const DataType &data) const;
    bool _Insert(Node **node, const DataType &val);
    bool _Remove(Node **node, const DataType &val);
    void _Clear(Node **node);
    bool _Find(const Node *node, const DataType &val) const;
    void _CopyNodes(Node **dest, const Node *source);
    void _BreadthFirstTraverse(const Node *node, CallBackFunc func) const;
    void _InOrderTraverse(const Node *node, CallBackFunc func) const;
    void _PostOrderTraverse(const Node *node, CallBackFunc func) const;

    Comparator m_comparator;
    EqualityComp m_equalityComp;
    UINT64 m_count;
    Node  *m_root;
  };

  template <class DataType, class Comparator, class EqualityComp>
  BST<DataType, Comparator, EqualityComp>::
    BST(Comparator comparator = Comparator(), EqualityComp equalityComp = EqualityComp())
    : m_count(0), m_root(nullptr), m_comparator(comparator), 
      m_equalityComp(equalityComp)
  {

  }

  template <class DataType, class Comparator, class EqualityComp>
  BST<DataType, Comparator, EqualityComp>::BST(const BST &other)
  {
    m_comparator = other.m_comparator;
    m_equalityComp = other.m_equalityComp;

    m_count = other->m_count;

    _CopyNodes(&m_root, nullptr, other->m_root);
  }

  template <class DataType, class Comparator, class EqualityComp>
  BST<DataType, Comparator, EqualityComp>::~BST(void)
  {
    this->Clear();
  }

  template <class DataType, class Comparator, class EqualityComp>
  const typename BST<DataType, Comparator, EqualityComp>&
    BST<DataType, Comparator, EqualityComp>::operator=(const BST &other)
  {
    this->Clear();

    m_comparator = other.m_comparator;
    m_equalityComp = other.m_equalityComp;

    m_count = other.m_count;
    _CopyNodes(&m_root, other->m_root);

    return *this;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    Insert(const typename DataType &val)
  {
    bool result = _Insert(&m_root, val);
    if(result)
      ++m_count;

    return result;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    Remove(const typename DataType &val)
  {
    bool result = _Remove(&m_root, val);
    if (result)
      --m_count;

    return result;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::Clear(void)
  {
    _Clear(&m_root);
    m_count = 0;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    Find(const typename DataType &data) const
  {
    return _Find(m_root, data);
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::Empty(void) const
  {
    return m_count == 0;
  }

  template <class DataType, class Comparator, class EqualityComp>
  UINT64 BST<DataType, Comparator, EqualityComp>::Size(void) const
  {
    return m_count;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    BreadthFirstTraverse(CallBackFunc func) const
  {
    _BreadthFirstTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    InOrderTraverse(CallBackFunc func) const
  {
    _InOrderTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    PostOrderTraverse(CallBackFunc func) const
  {
    _PostOrderTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  typename BST<DataType, Comparator, EqualityComp>::Node*
    BST<DataType, Comparator, EqualityComp>::
    _CreateNode(const typename DataType &data) const
  {
    Node *newNode = new Node();

    newNode->m_data = data;
    newNode->m_left = newNode->m_right = nullptr;

    return newNode;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    _Insert(Node **node, const typename DataType &val)
  {
    if (*node == nullptr)
    {
      *node = _CreateNode(val);
      return true;
    }

    if (m_equalityComp((*node)->m_data, val))
      return false;

    if (m_comparator(val, (*node)->m_data))
      return _Insert(&(*node)->m_left, val);
    else
      return _Insert(&(*node)->m_right, val);
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    _Remove(Node **node, const DataType &val)
  {
    if ((*node) == nullptr)
      return false;

    if (m_comparator(val, (*node)->m_data))
      return _Remove(&(*node)->m_left, val);
    else if (m_comparator((*node)->m_data, val))
      return _Remove(&(*node)->m_right, val);
    else
    {
      if ((*node)->m_left == nullptr)
      {
        Node *temp = *node;
        *node = (*node)->m_right;
        delete temp;

        return true;
      }
      else if ((*node)->m_right == nullptr)
      {
        Node *temp = *node;
        *node = (*node)->m_left;
        delete temp;

        return true;
      }
      else
      {
        Node *predecessor = (*node)->m_left;
        while (predecessor->m_right)
          predecessor = predecessor->m_right;

        (*node)->m_data = predecessor->m_data;

        return _Remove(&((*node)->m_left), predecessor->m_data);
      }
    }
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::_Clear(Node **node)
  {
    if ((*node) == nullptr)
      return;

    _Clear(&(*node)->m_left);
    _Clear(&(*node)->m_right);

    delete *node;
    *node = nullptr;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool BST<DataType, Comparator, EqualityComp>::
    _Find(const Node *node, const typename DataType &val) const
  {
    if (node == nullptr)
      return false;

    if (m_comparator(val, node->m_data))
      return _Find(node->m_left, val);
    if (m_comparator(node->m_data, val))
      return _Find(node->m_right, val);

    return true;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    _CopyNodes(Node **dest, const Node *source)
  {
    if (!source)
      return;

    *dest = _CreateNode(source->m_data);

    _CopyNodes(dest->m_left, source->m_left);
    _CopyNodes(dest->m_right, source->m_right);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    _BreadthFirstTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    func(node);
    _BreadthFirstTraverse(node->m_left, func);
    _BreadthFirstTraverse(node->right, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    _InOrderTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    _BreadthFirstTraverse(node->m_left, func);
    func(node);
    _BreadthFirstTraverse(node->right, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void BST<DataType, Comparator, EqualityComp>::
    _PostOrderTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    _BreadthFirstTraverse(node->m_left, func);
    _BreadthFirstTraverse(node->right, func);
    func(node);
  }
}