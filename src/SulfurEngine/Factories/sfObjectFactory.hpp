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

#pragma once

#include "Types\sfSingleton.hpp"
#include "DataStructures\sfSlotMap.hpp"
#include "Types\sfObject.hpp"

#undef GetObject

namespace Sulfur
{
  class ObjectFactory
  {
    SF_SINGLETON(ObjectFactory)

  public:
    Object* CreateObject(const std::string &name = "DefaultObject");
    //Without Transform
    Object* CreateEmptyObject(const std::string &name = "DefaultObject");

    Object* GetObject(const HNDL object) const;
    void DestroyObject(const HNDL object);

    void EndFrameCleanUp(void);

    void Serialize(std::ostream& str) const;
    void Deserialize(std::istream& str);

  private:
    void _Destroy(Object *obj);

    int numObjs = 0;
    std::vector<HNDL> m_objectsToDelete;
    SlotMap<Object> m_objects;
  };

#define SF_CREATE_EMPTY_OBJECT(Name) \
Sulfur::ObjectFactory::Instance()->CreateEmptyObject(Name)

#define SF_CREATE_OBJECT(Name) \
Sulfur::ObjectFactory::Instance()->CreateObject(Name)

#define SF_GET_OBJECT(Handle) \
Sulfur::ObjectFactory::Instance()->GetObject(Handle)
}