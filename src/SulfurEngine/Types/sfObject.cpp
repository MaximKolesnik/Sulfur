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

#include "sfObject.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  Object::Object(void)
  {

  }

  Object::~Object(void)
  {

  }

  void Object::Initialize(void)
  {

  }

  Object* Object::Clone(void) const
  {
    Object *clone = SF_CREATE_EMPTY_OBJECT(m_name);
    clone->SetParent(this->m_owner);

    //Clone components
    for (auto &it : m_components)
    {
      IEntity *compToClone = ComponentFactory::Instance()->GetComponent(it.first, it.second);

      clone->AttachComponent(compToClone->Clone());
    }

    _CloneChildren(clone, m_children);

    return clone;
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
        auto &newParentChildren = ObjectFactory::Instance()->GetObject(m_owner)->m_children;

        SF_ASSERT(newParentChildren.find(m_hndl) == newParentChildren.end(),
          "Object is already set as child");

        newParentChildren[m_hndl] = this;
      }
    }
    else //Parent is not set
    {
      auto &newParentChildren = ObjectFactory::Instance()->GetObject(m_owner)->m_children;

      SF_ASSERT(newParentChildren.find(m_hndl) == newParentChildren.end(),
        "Object is already set as child");

      newParentChildren[m_hndl] = this;
    }

    m_owner = parent;
  }

  void Object::AttachComponent(IEntity *component)
  {
    SF_ASSERT(m_components.find(component->m_name) == m_components.end(),
      component->m_name + " is already attached");

    component->m_owner = m_hndl;

    m_components[component->m_name] = component->m_hndl;
  }

  bool Object::HasDescendant(HNDL handle) const
  {
    for (auto &it : m_children)
    {
      if (it.first == handle)
        return true;
      else if (it.second->HasDescendant(handle))
        return true;
    }

    return false;
  }

  void Object::_CloneChildren(Object *parent, const ChildrenMap &children) const
  {
    for (auto &it : children)
    {
      Object *cloneChild = it.second->Clone();
      cloneChild->SetParent(parent->m_hndl);
    }
  }
}