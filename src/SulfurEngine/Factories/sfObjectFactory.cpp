/******************************************************************************/
/*!
\par     Sulfur
\file    sfObjectFactory.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/11/2016

\brief   Game object factory

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfObjectFactory.hpp"
#include "sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Reflection/sfSerialization.hpp"

namespace Sulfur
{
  ObjectFactory::ObjectFactory(void)
  {
  }

  ObjectFactory::~ObjectFactory(void)
  {
  }

  Object* ObjectFactory::CreateObject(const std::string &name)
  {
    Object *newObj = CreateEmptyObject(name);
    Transform *trans = SF_CREATE_COMP(Transform);

    newObj->AttachComponent(trans);
    trans->Initialize();

    return newObj;
  }

  Object* ObjectFactory::CreateEmptyObject(const std::string &name)
  {
    HNDL handle = m_objects.Create();
    SF_ASSERT(handle != SF_INV_HANDLE, "Object creation failed");

    Object *newObj = m_objects.At(handle);
    SF_ASSERT(newObj != nullptr, "Invalid object returned during creation");

    newObj->m_name = name;
    newObj->m_hndl = handle;

    return newObj;
  }

  Object* ObjectFactory::GetObject(const HNDL object) const
  {
    SF_ASSERT(object != SF_INV_HANDLE, "Invalid handle");
    return m_objects.At(object);
  }

  void ObjectFactory::DestroyObject(const HNDL object)
  {
    SF_ASSERT(object != SF_INV_HANDLE, "Invalid handle");

    m_objectsToDelete.push_back(object);
  }

  void ObjectFactory::EndFrameCleanUp(void)
  {
    for (auto &handle : m_objectsToDelete)
    {
      Object *obj = this->GetObject(handle);
      if (obj) //Object may already be deleted
        _Destroy(obj);
    }

    m_objectsToDelete.clear();
  }

  void ObjectFactory::Serialize(std::ostream& str) const
  {
    Serialization::Serialize(str, m_objects);
  }

  void ObjectFactory::Deserialize(std::istream& str)
  {
    for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
      DestroyObject(it->GetHndl());
    EndFrameCleanUp();

    Serialization::Deserialize(str, m_objects);
  }

  void ObjectFactory::_Destroy(Object *obj)
  {
    if (!obj->m_children.empty())
    {
      auto childIt = obj->m_children.begin();
      while (childIt != obj->m_children.end())
      {
        Object *child = GetObject(*childIt);

        ++childIt;

        _Destroy(child);
      }
    }

    if (obj->m_owner != SF_INV_HANDLE)
    {
      Object *parent = GetObject(obj->m_owner);
      parent->m_children.erase(obj->m_hndl);
    }

    auto compIt = obj->m_components.begin();
    while (compIt != obj->m_components.end())
    {
      std::string name = compIt->first;
      HNDL handle = compIt->second;
      ++compIt;
      
      g_SystemTable->CompFactory->DeleteComponent(name, handle);
    }

    m_objects.Erase(obj->m_hndl);
  }
}