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

#include <unordered_set>

#include "Types\sfIEntity.hpp"
#include "Types\sfTypes.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Reflection/sfSerialization.hpp"
#include "Events\sfEvents.hpp"

namespace Sulfur
{
  class ObjectFactory;
  class ComponentFactory;

  //Owner handle from IEntity is used to identify parent
  //SF_INV_HANDLE means object has no parent
  SF_REFLECTED_CLASS_DERIVED(Object, IEntity)
  public:
    typedef std::unordered_map<std::string, HNDL> ComponentMap;
    typedef std::unordered_set<HNDL> ChildrenSet;

    Object(void);
    virtual ~Object(void);

    virtual void Initialize(void) override final;
    virtual Object* Clone(void) const override final;
    virtual void Update(void) override final {};

    bool IsAlive(void) const { return m_isAlive; }

    void Destroy(void);

    void SetParent(HNDL parent);
    HNDL GetParent(void) const { return m_owner; }

    void AttachComponent(IEntity *component);

    bool HasDescendant(HNDL handle) const;

    bool HasComponent(const std::string &compType) const;
    template <class CompType>
    bool HasComponent(void) const;

    bool HasComponentOfGroup(const std::string &group) const;

    HNDL GetComponentHandle(const std::string &compType) const;
    template <class CompType>
    HNDL GetComponentHandle(void) const;

    IEntity* GetComponent(const std::string &compType) const;
    template <class CompType>
    CompType* GetComponent(void) const;

  private:
    friend class ObjectFactory;
    friend class ComponentFactory;

    std::string _RemoveScope(const std::string name) const;
    Object* _Clone(const Object *obj) const;

    void _CloneChildren(Object *parent, const ChildrenSet &children) const;

    SF_PRIVATE_PROPERTY(ComponentMap, components, Components, "Components");
    SF_PRIVATE_PROPERTY(ChildrenSet, children, Children, "Children");

    bool m_isAlive;

    SF_OBJECT_EVENT
  };

  template <class CompType>
  bool Object::HasComponent(void) const
  {
    std::string compType = _RemoveScope(typeid(CompType).name());

    SF_ASSERT(ComponentFactory::Instance()->IsRegistered(compType),
      compType + " is not registered");

    auto res = m_components.find(compType);

    if (res != m_components.end())
      return true;
    return false;
  }

  template <class CompType>
  HNDL Object::GetComponentHandle(void) const
  {
    std::string compType = _RemoveScope(typeid(CompType).name());

    auto res = m_components.find(compType);

    if (res != m_components.end())
      return res->second;

    return SF_INV_HANDLE;
  }

  template <class CompType>
  CompType* Object::GetComponent(void) const
  {
    std::string compType = _RemoveScope(typeid(CompType).name());

    auto res = m_components.find(compType);

    if (res != m_components.end())
      return ComponentFactory::Instance()->GetComponent<CompType>(res->second);
    return nullptr;
  }

}