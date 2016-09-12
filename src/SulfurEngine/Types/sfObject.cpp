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

namespace Sulfur
{
  Object::Object(void) : m_parent(SF_INV_HANDLE)
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
    SF_CRITICAL_ERR("GameObject::Clone is not implemented");
    return nullptr;
  }

  void Object::SetParent(HNDL parent)
  {
    if (m_parent != SF_INV_HANDLE) //Parent is set
    {
      Object *parentObj = ObjectFactory::Instance()->GetObject(m_parent);
#ifdef _DEBUG
      if (parent == m_parent 
        && parentObj->m_children.find(m_hndl) == parentObj->m_children.end())
        SF_ASSERT(false, "Already set is a parent, but this object is not set as a child");
      if (parent != m_parent 
        && parentObj->m_children.find(m_hndl) == parentObj->m_children.end())
        SF_ASSERT(false, "Not set as parent, but this object is set as a child");
#endif
      if (parent == m_parent)
        return;
      else
      {
        parentObj->m_children.erase(m_hndl);
        auto &newParentChildren = ObjectFactory::Instance()->GetObject(m_parent)->m_children;

        SF_ASSERT(newParentChildren.find(m_hndl) == newParentChildren.end(),
          "Object is already set as child");

        newParentChildren[m_hndl] = this;
      }
    }
    else //Parent is not set
    {
      auto &newParentChildren = ObjectFactory::Instance()->GetObject(m_parent)->m_children;

      SF_ASSERT(newParentChildren.find(m_hndl) == newParentChildren.end(),
        "Object is already set as child");

      newParentChildren[m_hndl] = this;
    }

    m_parent = parent;
  }
}