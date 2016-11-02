/******************************************************************************/
/*!
\par     Sulfur
\file    sfObject.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/11/2016

\brief   Game object

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>

#include "sfObject.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"

namespace Sulfur
{

SF_SCRIPT_TYPE_DERIVED(sulfur, Object, IEntity)
  SF_SCRIPT_MEMBER_FUNCTION(IsAlive)
  SF_SCRIPT_MEMBER_FUNCTION(Destroy)
  SF_SCRIPT_MEMBER_FUNCTION(SetParent)
  SF_SCRIPT_MEMBER_FUNCTION(GetParent)
SF_SCRIPT_TYPE_END()

Object::Object(void) : m_isAlive(true)
{
  ob_base = *(PyObject*)(&PythonTypeData<Object>::TypeObject);
}

Object::~Object(void)
{

}

void Object::Initialize(void)
{

}

Object* Object::Clone(void) const
{
  return _Clone(this);
}

void Object::Destroy(void)
{
  ObjectFactory::Instance()->DestroyObject(this);
}

void Object::SetParent(HNDL parent)
{
  //Should it be an assert?
  SF_ASSERT(!this->HasDescendant(parent), "Cannot set child object as parent");

  if (m_owner != SF_INV_HANDLE) //Parent is set
  {
    Object *parentObj = ObjectFactory::Instance()->GetObject(m_owner);
#ifdef _DEBUG
    if (parent == m_owner
      && parentObj->m_children.find(m_hndl) == parentObj->m_children.end())
      SF_ASSERT(false, "Already set is a parent, but this object is not set as a child");
    if (parent != m_owner
      && parentObj->m_children.find(m_hndl) == parentObj->m_children.end())
      SF_ASSERT(false, "Not set as parent, but this object is set as a child");
#endif
    if (parent == m_owner)
      return;
    else
    {
      parentObj->m_children.erase(m_hndl);
    }
  }
    
  if (parent != SF_INV_HANDLE)
  {
    auto &newParentChildren = ObjectFactory::Instance()->GetObject(parent)->m_children;

    SF_ASSERT(newParentChildren.find(m_hndl) == newParentChildren.end(),
      "Object is already set as child");

    newParentChildren.insert(m_hndl);
  }

  m_owner = parent;

  Transform *transform = GetComponent<Transform>();
  transform->Reparent();
}

void Object::AttachComponent(IEntity *component)
{
  SF_ASSERT(m_components.find(component->m_name) == m_components.end(),
    component->m_name + " is already attached");

  component->m_owner = m_hndl;

  m_components[component->m_name] = component->m_hndl;

  component->Initialize();
}

bool Object::HasDescendant(HNDL handle) const
{
  for (auto &it : m_children)
  {
    if (it == handle)
      return true;
    else if (SF_GET_OBJECT(it)->HasDescendant(handle))
      return true;
  }

  return false;
}

bool Object::HasComponent(const std::string &compType) const
{
  SF_ASSERT(ComponentFactory::Instance()->IsRegistered(compType),
    compType + " is not registered");

  auto res = m_components.find(compType);

  if (res != m_components.end())
    return true;
  return false;
}

HNDL Object::GetComponentHandle(const std::string &compType) const
{
  auto res = m_components.find(compType);

  if (res != m_components.end())
    return res->second;
    
  return SF_INV_HANDLE;
}

IEntity* Object::GetComponent(const std::string &compType) const
{
  auto res = m_components.find(compType);

  if (res != m_components.end())
    return SF_GET_COMP_STR(compType, res->second);
  return nullptr;
}

std::string Object::_RemoveScope(const std::string name) const
{
  size_t scopePos = name.find_last_of(":");
  if (scopePos == std::string::npos)
    return name;
  else
    return name.substr(scopePos + 1);
}

Object*Object::_Clone(const Object *obj) const
{
  Object *clone = SF_CREATE_EMPTY_OBJECT(m_name);

  if (obj->m_owner != SF_INV_HANDLE)
    clone->SetParent(obj->m_owner);

  //Clone components
  for (auto &it : m_components)
  {
    IEntity *compToClone = ComponentFactory::Instance()->GetComponent(it.first, it.second);

    clone->AttachComponent(compToClone->Clone());
  }

  _CloneChildren(clone, m_children);

  return clone;
}

void Object::_CloneChildren(Object *parent, const ChildrenSet &children) const
{
  for (auto &it : children)
  {
    Object *cloneChild = SF_GET_OBJECT(it)->_Clone(parent);
    cloneChild->SetParent(parent->m_hndl);
  }
}

}