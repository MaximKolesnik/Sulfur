/******************************************************************************/
/*!
\par     Sulfur
\file    sfAVL.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    8/28/2016

\brief   AVL tree

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../Types/sfTypes.hpp"

namespace Sulfur
{
  template <class DataType, class Comparator = std::less<DataType>,
            class EqualityComp = std::equal_to<DataType> >
  class AVL
  {
  public:
    struct Node
    {
      DataType m_data;

      INT64    m_balanceFactor;

      Node    *m_parent;
      Node    *m_left;
      Node    *m_right;
    };
    typedef bool(*CallBackFunc)(const Node *data);

    AVL(Comparator comparator = Comparator(),
      EqualityComp equalityComp = EqualityComp());
    AVL(const AVL &other);
    ~AVL(void);

    const AVL& operator=(const AVL &other);

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
    void _UpdateBalance(Node *node);
    void _Rebalance(Node **node);
    void _RotateRight(Node *node);
    void _RotateLeft(Node *node);
    INT64 _Height(const Node *node) const;
    Node* _CreateNode(Node *parent, const typename DataType &data) const;
    bool _Insert(Node **node, Node *parent, const DataType &val);
    void _CopyNodes(Node **dest, Node *parent, const Node *source);
    void _Clear(Node **node);
    bool _Find(const Node *node, const DataType &val) const;
    void _BreadthFirstTraverse(const Node *node, CallBackFunc func) const;
    void _InOrderTraverse(const Node *node, CallBackFunc func) const;
    void _PostOrderTraverse(const Node *node, CallBackFunc func) const;

    Comparator m_comparator;
    EqualityComp m_equalityComp;
    UINT64 m_count;
    Node  *m_root;
  };

  template <class DataType, class Comparator, class EqualityComp>
  AVL<DataType, Comparator, EqualityComp>::AVL(Comparator comparator = Comparator(),
    EqualityComp equalityComp = EqualityComp()) 
    : m_count(0), m_root(nullptr), m_comparator(comparator), 
      m_equalityComp(equalityComp)
  {

  }

  template <class DataType, class Comparator, class EqualityComp>
  AVL<DataType, Comparator, EqualityComp>::AVL(const AVL &other)
  {
    m_comparator = other.m_comparator;
    m_equalityComp = other.m_equalityComp;

    m_count = other.m_count;

    _CopyNodes(&m_root, nullptr, other.m_root);
  }

  template <class DataType, class Comparator, class EqualityComp>
  AVL<DataType, Comparator, EqualityComp>::~AVL(void)
  {
    this->Clear();
  }

  template <class DataType, class Comparator, class EqualityComp>
  const typename AVL<DataType, Comparator, EqualityComp>& 
    AVL<DataType, Comparator, EqualityComp>::operator=(const AVL &other)
  {
    this->Clear();

    m_comparator = other.m_comparator;
    m_equalityComp = other.m_equalityComp;

    m_count = other.m_count;
    _CopyNodes(&m_root, nullptr, other.m_root);

    return *this;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::
    Insert(const typename DataType &val)
  {
    bool result = _Insert(&m_root, nullptr, val);
    if (result)
      ++m_count;

    return result;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::
    Remove(const typename DataType &val)
  {
    Node *node = m_root, *parent = m_root, *delNode = nullptr,
      *child = m_root;

    while (child)
    {
      parent = node;
      node = child;
      child = (val >= node->m_data) ? node->m_right : node->m_left;

      if (m_equalityComp(val, node->m_data))
        delNode = node;
    }

    if (delNode)
    {
      delNode->m_data = node->m_data;
      child = node->m_left ? node->m_left : node->m_right;

      if (m_equalityComp(m_root->m_data, val))
        m_root = child;
      else
      {
        if (parent->m_left == node)
          parent->m_left = child;
        else
          parent->m_right = child;

        _UpdateBalance(parent);
      }

      delete node;
      --m_count;
      return true;
    }
    return false;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::Clear(void)
  {
    _Clear(&m_root);
    m_count = 0;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::
    Find(const typename DataType &data) const
  {
    return _Find(m_root, data);
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::Empty(void) const
  {
    return m_count == 0;
  }

  template <class DataType, class Comparator, class EqualityComp>
  UINT64 AVL<DataType, Comparator, EqualityComp>::Size(void) const
  {
    return m_count;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    BreadthFirstTraverse(CallBackFunc func) const
  {
    _BreadthFirstTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    InOrderTraverse(CallBackFunc func) const
  {
    _InOrderTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    PostOrderTraverse(CallBackFunc func) const
  {
    _PostOrderTraverse(m_root, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    _UpdateBalance(Node *node)
  {
    node->m_balanceFactor = _Height(node->m_left) - _Height(node->m_right);

    if (node->m_balanceFactor < -1 || node->m_balanceFactor > 1)
      _Rebalance(&node);

    if (node->m_parent)
      _UpdateBalance(node->m_parent);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    _Rebalance(Node **node)
  {
    if ((*node)->m_balanceFactor < 0)
    {
      if ((*node)->m_right->m_balanceFactor > 0)
      {
        _RotateRight((*node)->m_right);
        _RotateLeft(*node);
      }
      else
        _RotateLeft(*node);
    }
    else if ((*node)->m_balanceFactor > 0)
    {
      if ((*node)->m_left->m_balanceFactor < 0)
      {
        _RotateLeft((*node)->m_left);
        _RotateRight(*node);
      }
      else
        _RotateRight(*node);
    }
  }

  template <class DataType, class Comparator, class EqualityComp>
  INT64 AVL<DataType, Comparator, EqualityComp>::_Height(const Node *node) const
  {
    if (!node)
      return 0;

    INT64 lh = _Height(node->m_left);
    INT64 rh = _Height(node->m_right);

    return 1 + ((lh > rh) ? lh : rh);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::_RotateRight(Node *node)
  {
    Node *rotRoot = node->m_left;
    node->m_left = rotRoot->m_right;

    if (rotRoot->m_right)
      rotRoot->m_right->m_parent = node;
    rotRoot->m_parent = node->m_parent;

    if (node == m_root)
      m_root = rotRoot;
    else
    {
      if (node == node->m_parent->m_left)
        node->m_parent->m_left = rotRoot;
      else
        node->m_parent->m_right = rotRoot;
    }

    rotRoot->m_right = node;
    node->m_parent = rotRoot;

    rotRoot->m_balanceFactor = _Height(rotRoot->m_left) - _Height(rotRoot->m_right);
    node->m_balanceFactor = _Height(node->m_left) - _Height(node->m_right);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::_RotateLeft(Node *node)
  {
    Node *rotRoot = node->m_right;
    node->m_right = rotRoot->m_left;

    if (rotRoot->m_left)
      rotRoot->m_left->m_parent = node;
    rotRoot->m_parent = node->m_parent;

    if (node == m_root)
      m_root = rotRoot;
    else
    {
      if (node == node->m_parent->m_left)
        node->m_parent->m_left = rotRoot;
      else
        node->m_parent->m_right = rotRoot;
    }

    rotRoot->m_left = node;
    node->m_parent = rotRoot;
    
    rotRoot->m_balanceFactor = _Height(rotRoot->m_left) - _Height(rotRoot->m_right);
    node->m_balanceFactor = _Height(node->m_left) - _Height(node->m_right);
  }

  template <class DataType, class Comparator, class EqualityComp>
  typename AVL<DataType, Comparator, EqualityComp>::Node*
    AVL<DataType, Comparator, EqualityComp>::
    _CreateNode(Node *parent, const typename DataType &data) const
  {
    Node *newNode = new Node();

    newNode->m_parent = parent;
    newNode->m_balanceFactor = 0;
    newNode->m_data = data;
    newNode->m_left = newNode->m_right = nullptr;

    return newNode;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::_Insert(Node **node, Node *parent,
    const typename DataType &val)
  {
    if ((*node) == nullptr)
    {
      (*node) = _CreateNode(parent, val);
      _UpdateBalance(*node);
      return true;
    }

    if (m_comparator(val, (*node)->m_data))
      return _Insert(&(*node)->m_left, *node, val);
    else if (m_comparator((*node)->m_data, val))
      return _Insert(&(*node)->m_right, *node, val);

    return false;
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    _CopyNodes(Node **dest, Node *parent, const Node *source)
  {
    if (!source)
      return;

    *dest = _CreateNode(parent, source->m_data);

    _CopyNodes(&(*dest)->m_left, *dest, source->m_left);
    _CopyNodes(&(*dest)->m_right, *dest, source->m_right);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::_Clear(Node **node)
  {
    if ((*node) == nullptr)
      return;

    _Clear(&(*node)->m_left);
    _Clear(&(*node)->m_right);

    delete *node;
    (*node) = nullptr;
  }

  template <class DataType, class Comparator, class EqualityComp>
  bool AVL<DataType, Comparator, EqualityComp>::
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
  void AVL<DataType, Comparator, EqualityComp>::
    _BreadthFirstTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    func(node);
    _BreadthFirstTraverse(node->m_left, func);
    _BreadthFirstTraverse(node->right, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    _InOrderTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    _BreadthFirstTraverse(node->m_left, func);
    func(node);
    _BreadthFirstTraverse(node->right, func);
  }

  template <class DataType, class Comparator, class EqualityComp>
  void AVL<DataType, Comparator, EqualityComp>::
    _PostOrderTraverse(const Node *node, CallBackFunc func) const
  {
    if (!node)
      return;

    _BreadthFirstTraverse(node->m_left, func);
    _BreadthFirstTraverse(node->right, func);
    func(node);
  }
}