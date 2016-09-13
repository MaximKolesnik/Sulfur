/******************************************************************************/
/*!
\par     Sulfur
\file    sfObject.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/11/2016

\brief   Game object

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>

#include "Types\sfIEntity.hpp"
#include "Types\sfTypes.hpp"

namespace Sulfur
{
  class ObjectFactory;
  class ComponentFactory;

  //Owner handle from IEntity is used to identify parent
  //SF_INV_HANDLE means object has no parent
  class Object : public IEntity
  {
  public:
    typedef std::unordered_map<HNDL, Object*> ChildrenMap;

    Object(void);
    virtual ~Object(void);

    virtual void Initialize(void) override final;
    virtual Object* Clone(void) const override final;

    void SetParent(HNDL parent);
    HNDL GetParent(void) const { return m_owner; }

    void AttachComponent(IEntity *component);

    bool HasDescendant(HNDL handle) const;

  private:
    friend class ObjectFactory;
    friend class ComponentFactory;

    void _CloneChildren(Object *parent, const ChildrenMap &children) const;

    std::unordered_map<std::string, HNDL> m_components;

    ChildrenMap m_children;
  };
}