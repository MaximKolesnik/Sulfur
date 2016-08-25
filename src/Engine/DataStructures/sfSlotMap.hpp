/******************************************************************************/
/*!
\par     Sulfur
\file    sfSlotMap.hpp
\author  Dylan Norris
\par     DP email: d.norris\@digipen.edu
\date    8/23/2016

\brief   Templated slot map and handles

All content © 2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <deque>

#include "../Types/sfIEntity.h"
#include "../sfProjectDefs.hpp"
#include "../Types/sfTypes.hpp"
#include "../Error/sfError.hpp"

//Should be moved somewhere like GameSettings???
#define SF_MAX_OBJECTS    UINT64(2048)

namespace Sulfur
{
  class ISlotMap
  {
  public:
    ISlotMap(void)
    {
      for (UINT64 i = 0; i < SF_MAX_OBJECTS; ++i)
        m_freeList.push_back(i);
    }

    virtual ~ISlotMap(void) {};
    virtual hndl Create(void) = 0;
    virtual hndl CreateAt(UINT64 index) = 0;
    virtual void Clear(void) = 0;
    virtual IEntity* Get(const hndl handle) = 0;
    virtual void Erase(hndl handle) = 0;
    virtual UINT64 IndexOf(IEntity *object) = 0;

  protected:
    std::deque<UINT64> m_freeList;
    std::deque<UINT64> m_usedList;
  };

  template <typename ObjectType>
  class SlotMap : public ISlotMap
  {
  public:
    SlotMap() : ISlotMap()
    {
      m_objects = new ObjectType[SF_MAX_OBJECTS];

      SF_ASSERT(dynamic_cast<IEntity*>(m_objects) != nullptr,
        "Only types with bas IEntity can be put in SlotMap");
    }

    virtual ~SlotMap()
    {
      delete[]m_objects;
    }

    virtual hndl Create(void) override
    {
      SF_ASSERT(!m_freeList.empty(), "SlotMap is full");

      UINT64 index = m_freeList.front();
      m_freeList.pop_front();

      new (m_objects + index) ObjectType();
      m_usedList.push_back(index);

      return index;
    }

    virtual hndl CreateAt(UINT64 index) override
    {
      auto it = std::find(m_freeList.begin(), m_freeList.end(), index);
      SF_ASSERT(it != m_freeList.end(), "Slot is unavailable");

      m_freeList.erase(it);

      new (m_objects + index) ObjectType();
      m_usedList.push_back(index);

      return index;
    }

    virtual void Clear() override
    {
      m_freeList.clear();

      while (!m_usedList.empty())
      {
        ObjectType *object = m_objects + m_usedList.front();
        object->~ObjectType();
        m_usedList.pop_front();
      }

      for (UINT64 i = 0; i < SF_MAX_OBJECTS; ++i)
        m_freeList.push_back(i);
    }

    virtual ObjectType* Get(const hndl handle) override
    {
      if (!handle) return nullptr;

      UINT64 index = handle;
      if (index >= SF_MAX_OBJECTS) return nullptr;

      return &m_objects[handle];
    }

    virtual void Erase(hndl handle) override
    {
      auto it = std::find(m_usedList.begin(), m_usedList.end(), handle);
      SF_ASSERT(it != m_usedList.end(),
        "Trying to delete object in slot map that hasn't been initialized");

      m_usedList.erase(it);
      m_freeList.push_back(handle);
      m_objects[handle].~ObjectType();
      handle = SF_MAX_OBJECTS;
    }

    virtual UINT64 IndexOf(IEntity *object) override
    {
      return (UINT64)(static_cast<ObjectType*>(object) - m_objects);
    }

  private:
    ObjectType *m_objects;
  };

  /*template <typename ObjectType, SIZE_T MaxObjects>
  struct Handle
  {

    ENGINE_API Handle() : slotMapHandle(nullptr) {}
    ENGINE_API Handle(SIZE_T index) { slotMapHandle = MAKE_SLOT_MAP_HANDLE(index); }
    ENGINE_API Handle(ObjectType *object) { slotMapHandle = MAKE_SLOT_MAP_HANDLE(GetSlotMap().IndexOf(object)); }

    ENGINE_API static SlotMap<ObjectType, MaxObjects>& GetSlotMap(void)
    {
      static SlotMap<ObjectType, MaxObjects> instance;
      slotMap = &instance;
      return instance;
    }

    ENGINE_API operator bool() const { return (slotMapHandle && GetSlotMap().Get(slotMapHandle) != nullptr); }
    ENGINE_API operator SIZE_T() const { return *slotMapHandle; }

    ENGINE_API operator ObjectType*() { return GetSlotMap().Get(slotMapHandle); }
    ENGINE_API operator const ObjectType*() const { return GetSlotMap().Get(slotMapHandle); }

    ENGINE_API ObjectType* operator->() { return GetSlotMap().Get(slotMapHandle); }
    ENGINE_API const ObjectType* operator->() const { return GetSlotMap().Get(slotMapHandle); }

    ENGINE_API bool operator==(const Handle<ObjectType, MaxObjects>& rhs) { return *slotMapHandle == *(rhs.slotMapHandle); }
    ENGINE_API bool operator!=(const Handle<ObjectType, MaxObjects>& rhs) { return *slotMapHandle != *(rhs.slotMapHandle); }

    ENGINE_API static Handle Create()
    {
      Handle handle;
      handle.slotMapHandle = GetSlotMap().Create();
      return handle;
    }

    ENGINE_API static Handle CreateAt(SIZE_T index)
    {
      Handle handle;
      handle.slotMapHandle = GetSlotMap().CreateAt(index);
      return handle;
    }

    ENGINE_API static void FreeAll()
    {
      GetSlotMap().Clear();
    }

    SIZE_T GetIndex() const { return *slotMapHandle; }
    void SetIndex(SIZE_T index) { slotMapHandle = MAKE_SLOT_MAP_HANDLE(index); }

    void Delete() { GetSlotMap().Erase(slotMapHandle); }

  private:
    SlotMapHandle slotMapHandle;
    static SlotMap<ObjectType, MaxObjects> *slotMap;
  };*/
}