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
#include "Components\sfTransform.hpp"

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
        _Destroy(this->GetObject(handle));
    }
  }

  void ObjectFactory::_Destroy(Object *obj)
  {
    if (!obj->m_children.empty())
    {
      for (auto &it : obj->m_children)
        _Destroy(it.second);
    }

    for (auto &it : obj->m_components)
      ComponentFactory::Instance()->DeleteComponent(it.first, it.second);

    m_objects.Erase(obj->m_hndl);
  }
}