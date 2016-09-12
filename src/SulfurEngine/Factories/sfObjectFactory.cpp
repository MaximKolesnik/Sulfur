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
      //First destroy children
    }
  }

  void ObjectFactory::_Destroy(Object *obj)
  {
    if (obj->m_children.empty())
      return;



    m_objects.Erase(obj->m_hndl);
  }
}