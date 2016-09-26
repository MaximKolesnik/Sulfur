/******************************************************************************/
/*!
\par     Sulfur
\file    sfSlotMap.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik\@digipen.edu
\date    8/26/2016

\brief   Templated slot map. Used for Entities only.
         Memory Page is deallocated if there is more than one unused page
         Pages are deallocated only from the end

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

/*******************************************************************************
Maxim Kolesnik: TODO: Consider deallocating pages at any place in a list,
inerting missing handles when they are needed
                TODO: Use bitset for used list??
*******************************************************************************/

#pragma once

#include <list>
#include <vector>

#include "../Types/sfIEntity.hpp"
#include "../sfProjectDefs.hpp"
#include "../Types/sfTypes.hpp"
#include "../Settings/EngineSettings.h"
#include "../Error/sfError.hpp"
#include "sfOrderedDeque.hpp"

namespace Sulfur
{
  class ISlotMap
  {
  public:
    template <typename SlotMapType, typename EntityType>
    class IteratorBase
    {
    public:
      IteratorBase(SlotMapType *slotMap, HNDL index)
        : m_slotMap(slotMap), m_index(index)
      {
        HNDL lastHndl = m_slotMap->_LastHandle();
        while (m_index < lastHndl && m_slotMap->_IsFree(m_index))
          ++m_index;
      }

      IteratorBase& operator++(void)
      {
        HNDL lastHndl = m_slotMap->_LastHandle();
        SF_ASSERT(m_index != lastHndl, "Iterator is at end already");

        HNDL newIndex = m_index + 1;
        while (newIndex < lastHndl && m_slotMap->_IsFree(newIndex))
          ++newIndex;

        m_index = ((newIndex >= lastHndl) 
          ? m_slotMap->_LastHandle() : newIndex);

        return *this;
      }

      IteratorBase& operator++(int)
      {
        IteratorBase temp = *this;

        ++(*this);

        return temp;
      }

      IteratorBase& operator--(void)
      {
        SF_ASSERT(m_index != 0, "Iterator is at end already");

        HNDL newIndex = m_index - 1;
        while (m_slotMap->_IsFree(newIndex))
          --newIndex;

        m_index = ((newIndex < 0) ? 0 : newIndex);

        return *this;
      }

      IteratorBase& operator--(int)
      {
        IteratorBase temp = *this;

        --(*this);

        return temp;
      }

      EntityType* operator*(void) const
      {
        return m_slotMap->At(m_index);
      }

      EntityType* operator->(void) const
      {
        return m_slotMap->At(m_index);
      }

      IteratorBase& operator=(const IteratorBase &other)
      {
        m_slotMap = other.m_slotMap;
        m_index = other.m_index;
      }

      bool operator==(const IteratorBase &other) const
      {
        return m_index == other.m_index;
      }

      bool operator!=(const IteratorBase &other) const
      {
        return !(*this == other);
      }

    private:
      SlotMapType *m_slotMap;
      HNDL m_index;
    };

    typedef IteratorBase<ISlotMap, IEntity> Iterator;
    typedef IteratorBase<const ISlotMap, const IEntity> ConstIterator;

    ISlotMap(void)
      : m_size(0)
    {

    }

    UINT32 GetSize() const { return m_size; }

    virtual ~ISlotMap(void) {};
    virtual HNDL Create(void) = 0;
    virtual void CreateAt(const HNDL hndl) = 0;
    virtual IEntity* At(const HNDL hndl) const = 0;
    virtual void Erase(const HNDL hndl) = 0;
    virtual void Clear(void) = 0;
    virtual UINT64 GetNumberOfAllocPages(void) const = 0;
    virtual void Serialize(std::ostream& str) const = 0;
    virtual void Deserialize(std::istream& str) = 0;

    virtual Iterator begin(void) = 0;
    virtual Iterator end(void) = 0;
    virtual ConstIterator begin(void) const = 0;
    virtual ConstIterator end(void) const = 0;

  protected:
    virtual bool _IsFree(UINT64 pageNum, UINT64 index) const = 0;
    virtual bool _IsFree(HNDL handle) const = 0;
    virtual HNDL _LastHandle(void) const = 0;

    UINT32 m_size;
  };

  //Has to be IEntity or derived from it
  template <typename EntityType>
  class SlotMap : public ISlotMap
  {
  public:
    SlotMap(void);
    virtual ~SlotMap(void);

    virtual HNDL Create(void) override;
    virtual void CreateAt(const HNDL hndl) override;
    virtual EntityType* At(const HNDL hndl) const override;
    virtual void Erase(const HNDL hndl) override;
    virtual void Clear(void) override;
    virtual UINT64 GetNumberOfAllocPages(void) const override;
    virtual void Serialize(std::ostream& str) const override;
    virtual void Deserialize(std::istream& str) override;

    virtual Iterator begin(void) override
    {
      return Iterator(this, 0);
    }

    virtual Iterator end(void) override
    {
      return Iterator(this, _LastHandle());
    }

    virtual ConstIterator begin(void) const override
    {
      return ConstIterator(this, 0);
    }

    virtual ConstIterator end(void) const override
    {
      return ConstIterator(this, _LastHandle());
    }

  private:
    struct MemPage
    {
      EntityType *m_memory = nullptr;
      bool        m_full = false;
      UINT64      m_used = 0;

      HNDL        m_firstHndl = 0;

      std::list<HNDL> *m_freeList = nullptr;
    };

    struct Comparator
    {
      bool operator() (const MemPage *a, const MemPage *b) const
      {
        return a->m_firstHndl < b->m_firstHndl;
      }
    };

    typedef OrderedDeque<MemPage*, Comparator> PageQueue;

    MemPage* _AllocateNewPage(HNDL firstHndl);
    void _ConstructPageFreeList(HNDL firstHndl, std::list<HNDL> *&freeList);
    virtual bool _IsFree(UINT64 pageNum, UINT64 index) const override;
    virtual bool _IsFree(HNDL handle) const override;
    virtual HNDL _LastHandle(void) const override;

    MemPage *m_head = nullptr;
    std::vector<MemPage*> m_pages;
    PageQueue m_pageQueue; //pages that can be swapped with main free list
  };

  template <typename EntityType>
  SlotMap<EntityType>::SlotMap(void) : m_pageQueue(Comparator())
  {
    SF_CRITICAL_ERR_EXP(EngineSettings::SlotMapInitNumOfPages != 0,
      "Initial number of pages in SlotMap is 0");
    SF_CRITICAL_ERR_EXP(EngineSettings::SlotMapObjsPerPage > 1,
      "Number of objects per page is less than 2");

    //Create first page
    MemPage *newPage = _AllocateNewPage(0);
    m_head = newPage;

    m_pages.push_back(newPage);

    //Allocate rest of the pages
    for (int i = 1; i < EngineSettings::SlotMapInitNumOfPages; ++i)
    {
      MemPage *newPage = _AllocateNewPage(i * EngineSettings::SlotMapObjsPerPage);
      m_pages.push_back(newPage);
      m_pageQueue.push(newPage);
    }
  }

  template <typename EntityType>
  SlotMap<EntityType>::~SlotMap(void)
  {
    for (int i = 0; i < m_pages.size(); ++i)
    {
      free(m_pages[i]->m_memory);
      delete m_pages[i]->m_freeList;
    }
  }

  template <typename EntityType>
  HNDL SlotMap<EntityType>::Create(void)
  {
    SF_ASSERT(!m_head->m_freeList->empty(), "SlotMap: head free list is empty");

    HNDL newHndl = m_head->m_freeList->front();
    m_head->m_freeList->pop_front();

    //Locate the page and index
    UINT64 pageNum = newHndl / EngineSettings::SlotMapObjsPerPage;
    UINT64 index = newHndl % EngineSettings::SlotMapObjsPerPage;

    new (m_pages[pageNum]->m_memory + index) EntityType();

    //Swap free list if it is empty or create a new page
    if (m_head->m_freeList->empty())
    {
      m_pages[pageNum]->m_full = true;

      if (m_pageQueue.empty())
      {
        MemPage *newPage = _AllocateNewPage(m_pages.back()->m_firstHndl
          + EngineSettings::SlotMapObjsPerPage);
        m_pages.push_back(newPage);
        m_head = newPage;
      }
      else
      {
        m_head = m_pageQueue.front();
        m_pageQueue.pop_front();
      }
    }

    ++m_pages[pageNum]->m_used;
    ++m_size;

    SF_ASSERT(m_pages[pageNum]->m_used <= EngineSettings::SlotMapObjsPerPage,
      "Memory page indicates that it has more objects than objs per page");

    return newHndl;
  }

  template <typename EntityType>
  void SlotMap<EntityType>::CreateAt(const HNDL hndl)
  {
    SF_ASSERT(hndl != SF_INV_HANDLE, "Invalid handle");

    //Locate the page and index
    UINT64 pageNum = hndl / EngineSettings::SlotMapObjsPerPage;
    UINT64 index = hndl % EngineSettings::SlotMapObjsPerPage;

    if (m_pages.size() <= pageNum)
    {
      for (UINT64 i = m_pages.size(); i <= pageNum; ++i)
      {
        MemPage *newPage = _AllocateNewPage(m_pages.back()->m_firstHndl
          + EngineSettings::SlotMapObjsPerPage);
        m_pages.push_back(newPage);
        m_pageQueue.push(newPage);
      }
    }

    std::list<HNDL> *pageFreeList = m_pages[pageNum]->m_freeList;

    SF_CRITICAL_ERR_EXP(_IsFree(pageNum, index), "Handle is being used");
    SF_ASSERT(
      std::find(std::begin(*pageFreeList),std::end(*pageFreeList), hndl) 
      != std::end(*pageFreeList), "Handle is not on a free list");

    pageFreeList->remove(hndl);
    ++m_pages[pageNum]->m_used;
    ++m_size;

    SF_ASSERT(m_pages[pageNum]->m_used <= EngineSettings::SlotMapObjsPerPage,
      "Memory page indicates that it has more objects than objs per page");

    new (m_pages[pageNum]->m_memory + index) EntityType();
  }

  //Nullptr may be returned
  template <typename EntityType>
  typename EntityType* SlotMap<EntityType>::At(const HNDL hndl) const
  {
    SF_ASSERT(hndl != SF_INV_HANDLE, "Invalid hndl");

    //Locate the page and index
    UINT64 pageNum = hndl / EngineSettings::SlotMapObjsPerPage;
    UINT64 index = hndl % EngineSettings::SlotMapObjsPerPage;

    SF_ASSERT(m_pages.size() >= pageNum, "Object does not exist");

    if (_IsFree(pageNum, index))
    {
      SF_WARNING("SlotMap returned nullptr");
      return nullptr;
    }

    return &(m_pages[pageNum]->m_memory[index]);
  }

  template <typename EntityType>
  void SlotMap<EntityType>::Erase(const HNDL hndl)
  {
    //Locate the page and index
    UINT64 pageNum = hndl / EngineSettings::SlotMapObjsPerPage;
    UINT64 index = hndl % EngineSettings::SlotMapObjsPerPage;

    SF_ASSERT(m_pages.size() >= pageNum, "Object does not exist");
    SF_ASSERT(!_IsFree(pageNum, index), "Object does not exist");

    if (m_pages[pageNum]->m_full)
    {
      m_pages[pageNum]->m_full = false;
      //Swap free lists if current one has greater hndls
      if (m_head->m_firstHndl > hndl)
      {
        m_pageQueue.push(m_head);
        m_head = m_pages[pageNum];
      }
    }

    --m_pages[pageNum]->m_used;
    --m_size;
    SF_ASSERT(m_pages[pageNum]->m_used >= 0, 
      "Number of used object is less than 0");

    m_pages[pageNum]->m_memory[index].~EntityType();
    //Write dealloc pattern
    memset(reinterpret_cast<void*>(m_pages[pageNum]->m_memory + index),
      0, sizeof(EntityType));
    m_pages[pageNum]->m_freeList->push_front(hndl);

    //Try to deallocate pages
    SF_ASSERT(m_pageQueue.size() >= 1, "Page queue is empty");
    for (UINT64 i = m_pageQueue.size() - 1; 
      i >= EngineSettings::SlotMapInitNumOfPages - 1; --i)
    {
      //Leave one free page on the queue
      //Counting a page on the main free list
      //when we remove all elements we will have two empty pages
      if (m_pageQueue[i]->m_used == 0
        &&
        m_pageQueue[i - 1]->m_used == 0)
      {
        SF_ASSERT(m_pageQueue[i]->m_freeList->size() == EngineSettings::SlotMapObjsPerPage,
          "Deallocating not empty page");

        free(m_pageQueue[i]->m_memory);
        delete m_pageQueue[i]->m_freeList;

        m_pageQueue.pop_back();
        //Pages in the queue are sorted, so remove back element
        m_pages.pop_back();
        continue;
      }
      //If last page was not deallocated, than most likely there is
      //nothing to deallocate
      else
        break;
    }
  }

  template <typename EntityType>
  void SlotMap<EntityType>::Clear(void)
  {
    //Restore default number of pages
    for (INT64 i = m_pages.size() - 1; i >= EngineSettings::SlotMapInitNumOfPages; --i)
    {
      delete m_pages[i]->m_freeList;
      free(m_pages[i]->m_memory);
      delete m_pages[i];
      m_pages.pop_back();
    }

    //Reset pages
    for (auto &it : m_pages)
    {
      it->m_full = false;
      it->m_used = 0;
      it->m_freeList->clear();
      memset((void*)(it->m_memory), 0,
        sizeof(EntityType) * EngineSettings::SlotMapObjsPerPage);

      for (HNDL i = it->m_firstHndl; i < 
        it->m_firstHndl + EngineSettings::SlotMapObjsPerPage; ++i)
        it->m_freeList->push_back(i);
    }
    m_pageQueue.clear();

    for (auto it : m_pages)
      m_pageQueue.push(it);

    m_size = 0;
  }

  template <typename EntityType>
  UINT64 SlotMap<EntityType>::GetNumberOfAllocPages(void) const
  {
    return m_pages.size();
  }

  template <typename EntityType>
  void SlotMap<EntityType>::Serialize(std::ostream& str) const
  {
    Serialization::Serialize(str, *this);
  }

  template <typename EntityType>
  void SlotMap<EntityType>::Deserialize(std::istream& str)
  {
    Serialization::Deserialize(str, *this);
  }

  template <typename EntityType>
  typename SlotMap<EntityType>::MemPage* 
    SlotMap<EntityType>::_AllocateNewPage(HNDL firstHndl)
  {
    MemPage *newPage = new MemPage();
    newPage->m_memory = 
      (EntityType*)malloc(sizeof(EntityType) * EngineSettings::SlotMapObjsPerPage);
   
    memset((void*)(newPage->m_memory), 0, 
      sizeof(EntityType) * EngineSettings::SlotMapObjsPerPage);

    newPage->m_freeList = new std::list<HNDL>();

    _ConstructPageFreeList(firstHndl, newPage->m_freeList);
    newPage->m_firstHndl = firstHndl;

    return newPage;
  }

  template <typename EntityType>
  void SlotMap<EntityType>::_ConstructPageFreeList(HNDL firstHndl, std::list<HNDL> *&freeList)
  {
    UINT64 maxHndls = firstHndl + EngineSettings::SlotMapObjsPerPage;
    for (UINT64 i = firstHndl; i < maxHndls; ++i)
      freeList->push_back(i);
  }

  template <typename EntityType>
  bool SlotMap<EntityType>::_IsFree(UINT64 pageNum, UINT64 index) const
  {
    static const char testBlock[sizeof(EntityType)];

    return !memcmp(reinterpret_cast<void*>(m_pages[pageNum]->m_memory + index), 
      testBlock, sizeof(EntityType));
  }

  template <typename EntityType>
  bool SlotMap<EntityType>::_IsFree(HNDL handle) const
  {
    //Locate the page and index
    UINT64 pageNum = handle / EngineSettings::SlotMapObjsPerPage;
    UINT64 index = handle % EngineSettings::SlotMapObjsPerPage;

    static const char testBlock[sizeof(EntityType)];

    return !memcmp(reinterpret_cast<void*>(m_pages[pageNum]->m_memory + index),
      testBlock, sizeof(EntityType));
  }

  template <typename EntityType>
  HNDL SlotMap<EntityType>::_LastHandle(void) const
  {
    return m_pages.back()->m_firstHndl + EngineSettings::SlotMapObjsPerPage;
  }
}