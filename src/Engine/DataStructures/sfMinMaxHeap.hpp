/******************************************************************************/
/*!
\par     Sulfur
\file    sfMinMaxHeap.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    9/03/2016

\brief   Min-Max heap

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#pragma once

#include <vector>

#include "../Types/sfTypes.hpp"
#include "../Error/sfError.hpp"
#include "../Math/sfMathDefs.hpp"

namespace Sulfur
{
  template <class T, class Comparator = std::less<T> >
  class MinMaxHeap
  {
  public:
    MinMaxHeap(size_t initialCapacity = 0, Comparator = Comparator());
    MinMaxHeap(const MinMaxHeap &other);

    const MinMaxHeap& operator=(const MinMaxHeap &other);

    //Test function
    bool IsMinMaxHeap(void) const;

    void Insert(const T &val);

    const T& GetMin(void) const;
    const T& GetMax(void) const;

    T ExtractMin(void);
    T ExtractMax(void);

    //Root is located at index 1
    //Null parent is located at 0
    size_t Size(void) const { return m_heap.size() - 1; }

  private:
    void _PercolateUp(const size_t index);
    void _PercolateDown(const size_t index);
    void _PercolateUpMin(const size_t index);
    void _PercolateUpMax(const size_t index);
    void _PercolateDownMin(const size_t index);
    void _PercolateDownMax(const size_t index);

    size_t _GetParent(const size_t childInd) const { return childInd / 2; }
    size_t _GetLeftChild(const size_t parentInd) const { return 2 * parentInd; }
    size_t _GetRightChild(const size_t parentInd) const { return 2 * parentInd + 1; }
    inline size_t _GetGrandParent(const size_t gChildInd) const;
    inline bool _IsChildOf(const size_t childInd, const size_t parentInd) const;
    inline bool _IsGrandChildOf(const size_t gChildInd, const size_t gParentInd) const;

    inline bool _HasChildren(const size_t parentInd) const;
    inline size_t _GetSmallestChild(const size_t parentInd) const;
    inline size_t _GetGreatestChild(const size_t parentInd) const;

    //will return 0 if there are no children or grandchildren
    inline size_t _GetSmallestGrandChild(const size_t parentInd) const;
    inline size_t _GetGreatestGrandChild(const size_t parentInd) const;

    inline bool _IsOnMinLevel(const size_t index) const;
    inline bool _IsOnMaxLevel(const size_t index) const;

    bool _IsMinMaxHeap(const size_t root, const T *min, const T *max) const;

    std::vector<T> m_heap;
    Comparator m_comparator;
  };

  template <class T, class Comparator>
  MinMaxHeap<T, Comparator>::MinMaxHeap(size_t initialCapacity = 0,
    Comparator comparator = Comparator())
    : m_heap(std::vector<T>(1)), m_comparator(comparator)
  {
    m_heap.reserve(initialCapacity + 1);
  }

  template <class T, class Comparator>
  MinMaxHeap<T, Comparator>::MinMaxHeap(const MinMaxHeap &other)
  {
    m_heap = other.m_heap;
    m_comparator = other.m_comparator;
  }

  template <class T, class Comparator>
  const MinMaxHeap<T, Comparator>& MinMaxHeap<T, Comparator>::
    operator=(const MinMaxHeap &other)
  {
    m_heap = other.m_heap;
    m_comparator = other.m_comparator;

    return *this;
  }

  template <class T, class Comparator>
  bool MinMaxHeap<T, Comparator>::IsMinMaxHeap(void) const
  {
    if (m_heap.size() > 1)
      return _IsMinMaxHeap(1, nullptr, nullptr);
    else
      return true;
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::Insert(const typename T &val)
  {
    m_heap.push_back(val);

    _PercolateUp(Size());
  }

  template <class T, class Comparator>
  const typename T& MinMaxHeap<T, Comparator>::GetMin(void) const
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");

    return m_heap[1];
  }

  template <class T, class Comparator>
  const typename T& MinMaxHeap<T, Comparator>::GetMax(void) const
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");
    size_t maxEl = _GetGreatestChild(1);

    if (maxEl == 0)
      return m_heap[1];

    return m_heap[maxEl];
  }

  template <class T, class Comparator>
  typename T MinMaxHeap<T, Comparator>::ExtractMin(void)
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");

    T temp = m_heap[1];

    std::swap(m_heap[1], m_heap[Size()]);
    m_heap.pop_back();

    _PercolateDown(1);

    return temp;
  }

  template <class T, class Comparator>
  typename T MinMaxHeap<T, Comparator>::ExtractMax(void)
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");
    size_t maxEl = _GetGreatestChild(1);

    if (maxEl == 0)
      maxEl = 1;
    
    T temp = m_heap[maxEl];

    std::swap(m_heap[maxEl], m_heap[Size()]);
    m_heap.pop_back();

    _PercolateDown(maxEl);

    return temp;
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateUp(const size_t index)
  {
    size_t parentInd = _GetParent(index);

    if (_IsOnMinLevel(index))
    {
      if (parentInd != 0 && m_comparator(m_heap[parentInd], m_heap[index]))
      {
        std::swap(m_heap[parentInd], m_heap[index]);
        _PercolateUpMax(parentInd);
      }
      else
        _PercolateUpMin(index);
    }
    else
    {
      if (parentInd != 0 && m_comparator(m_heap[index], m_heap[parentInd]))
      {
        std::swap(m_heap[parentInd], m_heap[index]);
        _PercolateUpMin(parentInd);
      }
      else
        _PercolateUpMax(index);
    }
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateDown(const size_t index)
  {
    if (_IsOnMinLevel(index))
      _PercolateDownMin(index);
    else
      _PercolateDownMax(index);
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateUpMin(const size_t index)
  {
    size_t gParentInd = _GetGrandParent(index);
    if (gParentInd != 0)
    {
      if (m_comparator(m_heap[index], m_heap[gParentInd]))
      {
        std::swap(m_heap[index], m_heap[gParentInd]);
        _PercolateUpMin(gParentInd);
      }
    }
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateUpMax(const size_t index)
  {
    size_t gParentInd = _GetGrandParent(index);
    if (gParentInd != 0)
    {
      if (m_comparator(m_heap[gParentInd], m_heap[index]))
      {
        std::swap(m_heap[index], m_heap[gParentInd]);
        _PercolateUpMax(gParentInd);
      }
    }
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateDownMin(const size_t index)
  {
    if (!_HasChildren(index))
      return;

    size_t smallest = _GetSmallestChild(index);
    size_t smallestGrandChild = _GetSmallestGrandChild(index);;
    if (smallestGrandChild != 0
      &&
      m_comparator(m_heap[smallestGrandChild], m_heap[smallest]))
    {
      smallest = smallestGrandChild;
    }

    if (m_comparator(m_heap[index], m_heap[smallest]))
      return;

    if (_IsGrandChildOf(smallest, index))
    {
      if (m_comparator(m_heap[smallest], m_heap[index]))
      {
        std::swap(m_heap[smallest], m_heap[index]);
        
        if (m_comparator(m_heap[_GetParent(smallest)], m_heap[smallest]))
          std::swap(m_heap[_GetParent(smallest)], m_heap[smallest]);

        _PercolateDownMin(smallest);
      }
    }
    else if (m_comparator(m_heap[smallest], m_heap[index]))
        std::swap(m_heap[smallest], m_heap[index]);
  }

  template <class T, class Comparator>
  void MinMaxHeap<T, Comparator>::_PercolateDownMax(const size_t index)
  {
    if (!_HasChildren(index))
      return;

    size_t greatest = _GetGreatestChild(index);
    size_t greatestGrandChild = _GetGreatestGrandChild(index);;
    if (greatestGrandChild != 0
      &&
      m_comparator(m_heap[greatest], m_heap[greatestGrandChild]))
    {
      greatest = greatestGrandChild;
    }

    if (m_comparator(m_heap[greatest], m_heap[index]))
      return;

    if (_IsGrandChildOf(greatest, index))
    {
      if (m_comparator(m_heap[index], m_heap[greatest]))
      {
        std::swap(m_heap[greatest], m_heap[index]);

        if (m_comparator(m_heap[greatest], m_heap[_GetParent(greatest)]))
          std::swap(m_heap[_GetParent(greatest)], m_heap[greatest]);

        _PercolateDownMax(greatest);
      }
    }
    else if (m_comparator(m_heap[index], m_heap[greatest]))
      std::swap(m_heap[greatest], m_heap[index]);
  }

  template <class T, class Comparator>
  inline size_t 
    MinMaxHeap<T, Comparator>::_GetGrandParent(const size_t gChildInd) const
  {
    size_t parentInd = _GetParent(gChildInd);
    if (parentInd != 0)
      return _GetParent(parentInd);

    return 0;
  }

  template <class T, class Comparator>
  inline bool MinMaxHeap<T, Comparator>::
    _IsChildOf(const size_t childInd, const size_t parentInd) const
  {
    if (_GetParent(childInd) == parentInd)
      return true;

    return false;
  }

  template <class T, class Comparator>
  inline bool MinMaxHeap<T, Comparator>::
    _IsGrandChildOf(const size_t gChildInd, const size_t gParentInd) const
  {
    if (_GetGrandParent(gChildInd) == gParentInd)
      return true;

    return false;
  }

  template <class T, class Comparator>
  inline bool MinMaxHeap<T, Comparator>::_HasChildren(const size_t parentInd) const
  {
    //Since we are inserting in level order, we only need to check if parent
    // has left child
    size_t childInd = _GetLeftChild(parentInd);
    if (childInd <= Size())
      return true;
    return false;
  }

  template <class T, class Comparator>
  inline size_t 
    MinMaxHeap<T, Comparator>::
    _GetSmallestChild(const size_t parentInd) const
  {
    size_t smallest = 0;
    size_t leftChild = _GetLeftChild(parentInd),
      rightChild = _GetRightChild(parentInd);

    if (leftChild <= Size())
    {
      smallest = leftChild;

      if (rightChild <= Size() && m_comparator(m_heap[rightChild], m_heap[smallest]))
        smallest = rightChild;
    }

    return smallest;
  }

  template <class T, class Comparator>
  inline size_t MinMaxHeap<T, Comparator>::
    _GetGreatestChild(const size_t parentInd) const
  {
    size_t greatest = 0;
    size_t leftChild = _GetLeftChild(parentInd),
      rightChild = _GetRightChild(parentInd);

    if (leftChild <= Size())
    {
      greatest = leftChild;

      if (rightChild <= Size() && m_comparator(m_heap[greatest], m_heap[rightChild]))
        greatest = rightChild;
    }

    return greatest;
  }

  //will return 0 if there are no children or grandchildren
  template <class T, class Comparator>
  inline size_t MinMaxHeap<T, Comparator>::
    _GetSmallestGrandChild(const size_t parentInd) const
  {
    size_t smallest = 0;
    size_t leftSmallest = _GetSmallestChild(_GetLeftChild(parentInd));
    size_t rightSmallest = _GetSmallestChild(_GetRightChild(parentInd));

    if (leftSmallest != 0)
    {
      smallest = leftSmallest;

      if (rightSmallest != 0 && m_comparator(m_heap[rightSmallest], m_heap[smallest]))
        smallest = rightSmallest;
    }

    return smallest;
  }

  template <class T, class Comparator>
  inline size_t MinMaxHeap<T, Comparator>::
    _GetGreatestGrandChild(const size_t parentInd) const
  {
    size_t greatest = 0;
    size_t leftGreatest = _GetGreatestChild(_GetLeftChild(parentInd));
    size_t rightGreatest = _GetGreatestChild(_GetRightChild(parentInd));

    if (leftGreatest != 0)
    {
      greatest = leftGreatest;

      if (rightGreatest != 0 && m_comparator(m_heap[greatest], m_heap[rightGreatest]))
        greatest = rightGreatest;
    }

    return greatest;
  }

  template <class T, class Comparator>
  inline bool MinMaxHeap<T, Comparator>::_IsOnMinLevel(const size_t index) const
  {
    return int(MathUtils::Log2((Real)index)) % 2 != 1;
  }

  template <class T, class Comparator>
  inline bool MinMaxHeap<T, Comparator>::_IsOnMaxLevel(const size_t index) const
  {
    return int(MathUtils::Log2((Real)index)) % 2 == 1;
  }

  template <class T, class Comparator>
  bool MinMaxHeap<T, Comparator>::
    _IsMinMaxHeap(const size_t root, const T *min, const T *max) const
  {
    if (root > Size()) //Base case
      return true;

    if (_IsOnMinLevel(root))
    {
      if (min != nullptr)
        if (m_comparator(m_heap[root], *min))
          return false;

      if (max != nullptr)
        if (m_comparator(*max, m_heap[root]))
          return false;

      min = &m_heap[root];

      size_t leftChild = _GetLeftChild(root);
      size_t rightChild = _GetRightChild(root);

      if (leftChild <= Size() && m_comparator(m_heap[leftChild], m_heap[root]))
        return false;

      if (rightChild <= Size() && m_comparator(m_heap[rightChild], m_heap[root]))
        return false;
    }
    else //Max level
    {
      if (min != nullptr)
        if (m_comparator(m_heap[root], *min))
          return false;

      if (max != nullptr)
        if (m_comparator(*max, m_heap[root]))
          return false;

      max = &m_heap[root];

      size_t leftChild = _GetLeftChild(root);
      size_t rightChild = _GetRightChild(root);

      if (leftChild <= Size() && m_comparator(m_heap[root], m_heap[leftChild]))
        return false;

      if (rightChild <= Size() && m_comparator(m_heap[root], m_heap[rightChild]))
        return false;
    }

    return _IsMinMaxHeap(_GetLeftChild(root), min, max)
      && _IsMinMaxHeap(_GetRightChild(root), min, max);

  }
}