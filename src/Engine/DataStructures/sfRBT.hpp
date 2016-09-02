/******************************************************************************/
/*!
\par     Sulfur
\file    sfRBT.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    8/29/2016

\brief   Red-Black tree

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

namespace Sulfur
{
  template <typename DataType>
  class RBT
  {
  public:
    struct Node
    {
      enum Color { Red, Black };

      Color m_color = Red;

      DataType m_data;

      Node *m_left = nullptr;
      Node *m_right = nullptr;
    };

    RBT(void);
    RBT(const RBT &other);
    ~RBT(void);

    const RBT& operator=(const RBT &other);

    bool Insert(const DataType &val);
    bool Remove(const DataType &val);

    void Clear(void);

    bool Find(const DataType &val) const;
    bool Empty(void) const;
    UINT64 Size(void) const;

    const Node* GetRoot(void) const { return m_root; }

  private:
    Node* _Insert(Node *node, const DataType &val, bool &result);
    Node* _Remove(Node *node, DataType val, bool &result, bool &done);
    bool _IsRed(const Node *node) const;
    Node* _CreateNode(const DataType &val) const;
    Node* _RotateLeft(Node *node);
    Node* _RotateRight(Node *node);
    void _CopyNodes(Node **dest, const Node *source);
    void _Clear(Node **node);
    bool _Find(const Node *node, const typename DataType &val) const;
    Node* _RemoveRebalanceRight(Node *node, bool &done);
    Node* _RemoveRebalanceLeft(Node *node, bool &done);
    UINT64 m_count;
    Node *m_root;
  };

  template <typename DataType>
  RBT<DataType>::RBT(void) : m_count(0), m_root(nullptr)
  {

  }

  template <typename DataType>
  RBT<DataType>::RBT(const RBT &other)
  {
    m_count = other.m_count;

    _CopyNodes(&m_root, other.m_root);
  }

  template <typename DataType>
  RBT<DataType>::~RBT(void)
  {
    Clear();
  }

  template <typename DataType>
  const RBT<DataType>& RBT<DataType>::operator=(const RBT &other)
  {
    this->Clear();

    m_count = other.m_count;
    _CopyNodes(&m_root, other.m_root);

    return *this;
  }

  template <typename DataType>
  bool RBT<DataType>::Insert(const DataType &val)
  {
    bool res = false;
    m_root = _Insert(m_root, val, res);
    m_root->m_color = Node::Black;

    if (res)
      ++m_count;

    return res;
  }

  template <typename DataType>
  bool RBT<DataType>::Remove(const DataType &val)
  {
    bool done = false, result = false;

    m_root = _Remove(m_root, val, result, done);
    if (m_root)
      m_root->m_color = Node::Black;

    if (result)
      --m_count;

    return result;
  }

  template <typename DataType>
  void RBT<DataType>::Clear(void)
  {
    _Clear(&m_root);
    m_count = 0;
  }

  template <typename DataType>
  bool RBT<DataType>::Find(const DataType &val) const
  {
    return _Find(m_root, val);
  }

  template <typename DataType>
  bool RBT<DataType>::Empty(void) const
  {
    return m_count == 0;
  }

  template <typename DataType>
  UINT64 RBT<DataType>::Size(void) const
  {
    return m_count;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* 
    RBT<DataType>::_Insert(Node *node, typename const DataType &val, bool &result)
  {
    if (!node)
    {
      node = _CreateNode(val);
      result = true;
    }
    else if (val != node->m_data)
    {
      if (val < node->m_data)
      {
        node->m_left = _Insert(node->m_left, val, result);

        if (_IsRed(node->m_left))
        {
          if (_IsRed(node->m_right))
          {
            node->m_color = Node::Red;
            node->m_left->m_color = node->m_right->m_color = Node::Black;
          }
          else
          {
            if (_IsRed(node->m_left->m_left))
              node = _RotateRight(node);
            else if (_IsRed(node->m_left->m_right))
            {
              node->m_left = _RotateLeft(node->m_left);
              node = _RotateRight(node);
            }
          }
        }
      }
      else
      {
        node->m_right = _Insert(node->m_right, val, result);

        if (_IsRed(node->m_right))
        {
          if (_IsRed(node->m_left))
          {
            node->m_color = Node::Red;
            node->m_left->m_color = node->m_right->m_color = Node::Black;
          }
          else
          {
            if (_IsRed(node->m_right->m_right))
              node = _RotateLeft(node);
            else if (_IsRed(node->m_right->m_left))
            {
              node->m_right = _RotateRight(node->m_right);
              node = _RotateLeft(node);
            }
          }
        }
      }
    }

    return node;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* 
    RBT<DataType>::_Remove(Node *node, DataType val, bool &result, bool &done)
  {
    if (!node)
      done = true;
    else
    {
      if (node->m_data == val)
      {
        if (!node->m_left)
        {
          Node *temp = node->m_right;

          if (_IsRed(node))
            done = true;
          else if (_IsRed(temp))
          {
            temp->m_color = Node::Black;
            done = true;
          }

          delete node;

          result = true;

          return temp;
        }
        else if (!node->m_right)
        {
          Node *temp = node->m_left;

          if (_IsRed(node))
            done = 1;
          else if (_IsRed(temp))
          {
            temp->m_color = Node::Black;
            done = true;
          }

          delete node;

          result = true;

          return temp;
        }
        else
        {
          Node *pred = node->m_left;

          while (pred->m_right)
            pred = pred->m_right;

          node->m_data = pred->m_data;
          val = pred->m_data;
        }
      }

      if (node->m_data < val)
      {
        node->m_right = _Remove(node->m_right, val, result, done);

        if (!done)
          node = _RemoveRebalanceRight(node, done);
      }
      else
      {
        node->m_left = _Remove(node->m_left, val, result, done);

        if (!done)
          node = _RemoveRebalanceLeft(node, done);
      }
    }
    return node;
  }

  template <typename DataType>
  bool RBT<DataType>::_IsRed(const Node *node) const
  {
    return (node != nullptr) && node->m_color == Node::Red;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* 
    RBT<DataType>::_CreateNode(const typename DataType &val) const
  {
    Node *newNode = new Node();

    newNode->m_color = Node::Red;
    newNode->m_data = val;
    newNode->m_left = newNode->m_right = nullptr;

    return newNode;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* RBT<DataType>::_RotateLeft(Node *node)
  {
    Node *newRoot = node->m_right;
    node->m_right = newRoot->m_left;
    newRoot->m_left = node;

    node->m_color = Node::Red;
    newRoot->m_color = Node::Black;

    return newRoot;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* RBT<DataType>::_RotateRight(Node *node)
  {
    Node *newRoot = node->m_left;
    node->m_left = newRoot->m_right;
    newRoot->m_right = node;

    node->m_color = Node::Red;
    newRoot->m_color = Node::Black;

    return newRoot;
  }

  template <typename DataType>
  void RBT<DataType>::_CopyNodes(Node **dest, const Node *source)
  {
    if (!source)
      return;

    *dest = _CreateNode(source->m_data);
    (*dest)->m_color = source->m_color;

    _CopyNodes(&(*dest)->m_left, source->m_left);
    _CopyNodes(&(*dest)->m_right, source->m_right);
  }

  template <typename DataType>
  void RBT<DataType>::_Clear(Node **node)
  {
    if ((*node) == nullptr)
      return;

    _Clear(&(*node)->m_left);
    _Clear(&(*node)->m_right);

    delete *node;
    (*node) = nullptr;
  }

  template <typename DataType>
  bool RBT<DataType>::_Find(const Node *node, const typename DataType &val) const
  {
    if (node == nullptr)
      return false;

    if (val < node->m_data)
      return _Find(node->m_left, val);
    if (val > node->m_data)
      return _Find(node->m_right, val);

    return true;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* 
    RBT<DataType>::_RemoveRebalanceRight(Node *node, bool &done)
  {
    Node *parent = node;
    Node *sibling = node->m_left;

    if (_IsRed(sibling))
    {
      node = _RotateRight(node);
      sibling = parent->m_left;
    }

    if (sibling)
    {
      if (!_IsRed(sibling->m_left) && !_IsRed(sibling->m_right))
      {
        if (_IsRed(parent))
          done = true;

        parent->m_color = Node::Black;
        sibling->m_color = Node::Red;
      }
      else
      {
        Node::Color parentColor = parent->m_color;
        bool newRoot = (node == parent);

        if (_IsRed(sibling->m_left))
          parent = _RotateRight(parent);
        else
        {
          parent->m_left = _RotateLeft(parent->m_left);
          parent = _RotateRight(parent);
        }

        parent->m_color = parentColor;
        parent->m_left->m_color = Node::Black;
        parent->m_right->m_color = Node::Black;

        if (newRoot)
          node = parent;
        else
          node->m_right = parent;

        done = true;
      }
    }

    return node;
  }

  template <typename DataType>
  typename RBT<DataType>::Node* 
    RBT<DataType>::_RemoveRebalanceLeft(Node *node, bool &done)
  {
    Node *parent = node;
    Node *sibling = node->m_right;

    if (_IsRed(sibling))
    {
      node = _RotateLeft(node);
      sibling = parent->m_right;
    }

    if (sibling)
    {
      if (!_IsRed(sibling->m_left) && !_IsRed(sibling->m_right))
      {
        if (_IsRed(parent))
          done = true;

        parent->m_color = Node::Black;
        sibling->m_color = Node::Red;
      }
      else
      {
        Node::Color parentColor = parent->m_color;
        bool newRoot = (node == parent);

        if (_IsRed(sibling->m_right))
          parent = _RotateLeft(parent);
        else
        {
          parent->m_right = _RotateRight(parent->m_right);
          parent = _RotateLeft(parent);
        }

        parent->m_color = parentColor;
        parent->m_left->m_color = Node::Black;
        parent->m_right->m_color = Node::Black;

        if (newRoot)
          node = parent;
        else
          node->m_left = parent;

        done = true;
      }
    }

    return node;
  }
}