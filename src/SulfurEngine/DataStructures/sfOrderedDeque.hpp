/******************************************************************************/
/*!
\par     Sulfur
\file    sfOrderedDeque.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    8/26/2016

\brief   Ordered deque wrapped around std::deque

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <deque>

namespace Sulfur
{
  template <class T, class Functor, class Alloc = std::allocator<T> >
  class OrderedDeque
  {
  public:
    typedef typename OrderedDeque<T, Functor, Alloc> thisDeque;
    typedef typename std::deque<T, Alloc>::iterator iterator;
    typedef typename std::deque<T, Alloc>::const_iterator const_iterator;
    typedef typename std::deque<T, Alloc>::allocator_type allocator_type;
    typedef typename std::deque<T, Alloc>::pointer pointer;
    typedef typename std::deque<T, Alloc>::const_pointer const_pointer;
    typedef typename std::deque<T, Alloc>::value_type value_type;
    typedef typename std::deque<T, Alloc>::reference reference;
    typedef typename std::deque<T, Alloc>::const_reference const_reference;
    typedef typename std::deque<T, Alloc>::reverse_iterator reverse_iterator;
    typedef typename std::deque<T, Alloc>::const_reverse_iterator const_reverse_iterator;
    typedef typename std::deque<T, Alloc>::difference_type difference_type;
    typedef typename std::deque<T, Alloc>::size_type size_type;

    explicit OrderedDeque(Functor comparator, const allocator_type &alloc = allocator_type())
      : m_comparator(comparator), m_deque(alloc) {}

    explicit OrderedDeque(Functor comparator, size_type n)
      : m_comparator(comparator), m_deque(n)
    {

    }

    OrderedDeque(Functor comparator, size_type n, const value_type &val,
      const allocator_type &alloc = allocator_type()) 
      : m_comparator(comparator), m_deque(n, val, alloc)
    {

    }

    template <class InputIterator>
    OrderedDeque(Functor comparator, InputIterator first, InputIterator last,
      const allocator_type &alloc = allocator_type()) 
      : m_comparator(comparator), m_deque(first, last, alloc)
    {

    }

    OrderedDeque(const thisDeque &x)
      : m_comparator(x.m_comparator), m_deque(x.m_deque)
    {

    }
    OrderedDeque(const thisDeque &x,
      const allocator_type &alloc) : m_comparator(x.comparator), m_deque(x.m_deque, alloc)
    {

    }

    OrderedDeque(thisDeque &&x)
      : m_comparator(std::move(x.comparator)), m_deque(std::move(x.m_deque))
    {

    }
    OrderedDeque(thisDeque &&x, const allocator_type &alloc)
      : m_comparator(std::move(x.comparator)), m_deque(std::move(x.m_deque), alloc)
    {

    }

    OrderedDeque(Functor comparator, std::initializer_list<value_type> il,
      const allocator_type &alloc = allocator_type()) 
      : m_comparator(comparator), m_deque(il, alloc)
    {

    }

    ~OrderedDeque(void) {}

    thisDeque& operator=(const thisDeque &x)
    {
      m_comparator = x.m_comporator;
      m_deque = x.m_deque;

      return *this;
    }

    thisDeque& operator=(thisDeque &&x)
    {
      m_comparator = std::move(x.m_comporator);
      m_deque = std::move(x.m_deque);

      return *this;
    }

    thisDeque& operator=(std::initializer_list<value_type> il)
    {
      return m_deque = il;
    }

    size_type size() const
    {
      return m_deque.size();
    }

    size_type max_size() const
    {
      return m_deque.max_size();
    }

    void resize(size_type n, value_type val = value_type())
    {
      m_deque.resize(n, val);
    }

    bool empty() const
    {
      return m_deque.empty();
    }

    void shrink_to_fit(void)
    {
      m_deque.shrink_to_fit();
    }

    reference operator[](size_type n)
    {
      return m_deque[n];
    }

    const_reference operator[](size_type n) const
    {
      return m_deque[n];
    }

    reference at(size_type n)
    {
      return m_deque.at(n);
    }

    const_reference at(size_type n) const
    {
      return m_deque.at(n);
    }

    reference front()
    {
      return m_deque.front();
    }

    const_reference front() const
    {
      return m_deque.front();
    }

    reference back()
    {
      return m_deque.back();
    }

    const_reference back() const
    {
      return m_deque.back();
    }

    void push(const value_type &val)
    {
      if (m_deque.size() == 0)
      {
        m_deque.push_front(val);
        return;
      }

      for (const_iterator it = m_deque.begin(); it != m_deque.end(); ++it)
        if (m_comparator(val, *it))
        {
          m_deque.insert(it, val);
          return;
        }

      m_deque.push_back(val);
    }

    void push(value_type &&val)
    {
      if (m_deque.size() == 0)
      {
        m_deque.push_front(val);
        return;
      }

      for (const_iterator it = m_deque.begin(); it != m_deque.end(); ++it)
        if (m_comparator(val, *it))
        {
          m_deque.insert(it, val);
          return;
        }

      m_deque.push_back(val);
    }

    template <class InputIterator>
    void assign(typename InputIterator first, typename InputIterator last)
    {
      m_deque.assign(first, last);
    }

    void assign(size_type n, const value_type &val)
    {
      m_deque.assign(n, val);
    }

    void assign(std::initializer_list<value_type> il)
    {
      m_deque.assign(il);
    }

    void pop_back(void)
    {
      m_deque.pop_back();
    }

    void pop_front(void)
    {
      m_deque.pop_front();
    }

    iterator erase(const_iterator position)
    {
      m_deque.erase(position);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
      m_deque.erase(first, last);
    }

    void resize(size_type n)
    {
      m_deque.resize(n);
    }

    void resize(size_type n, const value_type &val)
    {
      m_deque.resize(n, val);
    }

    void clear(void) noexcept
    {
      m_deque.clear()
    }

  private:
    Functor              m_comparator;
    std::deque<T, Alloc> m_deque;
  };
}