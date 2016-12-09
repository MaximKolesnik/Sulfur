/******************************************************************************/
/*!
\par     Sulfur
\file    sfHeap.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    9/01/2016

\brief   Heap. Can be used to construct min or max heap, depending on
         given comparator

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <vector>

#include "../Types/sfTypes.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  template <class T, class Comparator = std::less<T> >
  class Heap
  {
  public:
    Heap(size_t initialCapacity = 0, Comparator comparator = Comparator());
    Heap(const Heap &other);

    const Heap& operator=(const Heap &other);

    //Test function
    virtual bool IsHeap(void) const;

    virtual void Insert(const T &val);
    virtual void Delete(const size_t index);

    //Depending on given comparator, could be min or max
    const T& GetRoot(void) const;
    T ExtractRoot(void);

    //Root is located at index 1
    //Null parent is located at 0
    size_t Size(void) const { return m_heap.size() - 1; }

  protected:
    virtual void _PercolateUp(const size_t index);
    virtual void _PercolateDown(const size_t index);
    size_t _GetParent(const size_t childInd) const { return childInd / 2; }
    size_t _GetLeftChild(const size_t parentInd) const { return 2 * parentInd; }
    size_t _GetRightChild(const size_t parentInd) const { return 2 * parentInd + 1;}

    std::vector<T> m_heap;
    Comparator m_comparator;

  private:
    bool _IsHeap(const size_t parent) const;
  };

  template <class T, class Comparator = std::less<T> >
  Heap<T, Comparator>::Heap(size_t initialCapacity = 0, 
    Comparator comparator = Comparator()) 
    : m_heap(std::vector<T>(1)), m_comparator(comparator)
  {
    m_heap.reserve(initialCapacity + 1);
  }

  template <class T, class Comparator>
  Heap<T, Comparator>::Heap(const Heap &other)
  {
    m_heap = ohter.m_heap;
    m_comparator = ohter.m_comparator;
  }

  template <class T, class Comparator>
  const typename Heap<T, Comparator>& 
    Heap<T, Comparator>::operator=(const Heap &other)
  {
    m_heap = ohter.m_heap;
    m_comparator = ohter.m_comparator;

    return *this;
  }

  template <class T, class Comparator = std::less<T> >
  bool Heap<T, Comparator>::IsHeap(void) const
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");

    return _IsHeap(1);
  }

  template <class T, class Comparator = std::less<T> >
  void Heap<T, Comparator>::Insert(const typename T &val)
  {
    m_heap.push_back(val);

    _PercolateUp(Size());
  }

  template <class T, class Comparator = std::less<T> >
  void Heap<T, Comparator>::Delete(const size_t index)
  {
    SF_ASSERT(m_heap.size() > index && index != 0, "Index overflow");

    std::swap(m_heap[index], m_heap[m_heap.size() - 1]);
    m_heap.pop_back();

    size_t parent = _GetParent(index);
    if (parent == 0)
    {
      _PercolateDown(index);
    }
    else
    {
      if (m_comparator(m_heap[index], m_heap[parent]))
        _PercolateUp(index);
      else
        _PercolateDown(index);
    }
  }

  template <class T, class Comparator = std::less<T> >
  const typename T& Heap<T, Comparator>::GetRoot(void) const
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");  
    return m_heap[1];
  }

  template <class T, class Comparator = std::less<T> >
  typename T Heap<T, Comparator>::ExtractRoot(void)
  {
    SF_ASSERT(m_heap.size() > 1, "Heap is empty");
    T temp = m_heap[1];

    Delete(1);

    return temp;
  }

  template <class T, class Comparator = std::less<T> >
  void Heap<T, Comparator>::_PercolateUp(const size_t index)
  {
    size_t child = index;
    size_t parent;
    while ((parent = _GetParent(child)) != 0)
    {
      if ( m_comparator(m_heap[child], m_heap[parent]) )
        std::swap(m_heap[parent], m_heap[child]);
      else 
        break;

      child = parent;
    }
  }

  template <class T, class Comparator = std::less<T> >
  void Heap<T, Comparator>::_PercolateDown(const size_t index)
  {
    size_t nextToPerlocate = 0, parent = index;
    size_t leftChild = _GetLeftChild(index);
    size_t rightChild = _GetRightChild(index);

    while (nextToPerlocate != parent)
    {
      nextToPerlocate = parent;

      if (leftChild < m_heap.size())
        if (m_comparator(m_heap[leftChild], m_heap[parent]))
          nextToPerlocate = leftChild;

      if (rightChild < m_heap.size())
        if (m_comparator(m_heap[rightChild], m_heap[nextToPerlocate]))
          nextToPerlocate = rightChild;

      if (nextToPerlocate != parent)
      {
        std::swap(m_heap[nextToPerlocate], m_heap[parent]);
        parent = nextToPerlocate;
        nextToPerlocate = 0;
        leftChild = _GetLeftChild(parent);
        rightChild = _GetRightChild(parent);
      }
    }
  }


  template <class T, class Comparator = std::less<T> >
  bool Heap<T, Comparator>::_IsHeap(const size_t parent) const
  {
    size_t lc = _GetLeftChild(parent);
    size_t rc = _GetRightChild(parent);
    size_t heapSize = m_heap.size();

    if (lc >= heapSize && rc >= heapSize)
      return true;

    if (rc >= heapSize)
      return m_comparator(m_heap[parent], m_heap[lc]);
    else
    {
      if (m_comparator(m_heap[parent], m_heap[lc]) 
        &&
        m_comparator(m_heap[parent], m_heap[rc]))
        return _IsHeap(lc) && _IsHeap(rc);
      else
        return false;
    }
  }
}