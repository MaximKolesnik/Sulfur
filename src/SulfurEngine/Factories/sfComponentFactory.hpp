/******************************************************************************/
/*!
\par     Sulfur
\file    sfComponentFactory.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Component Factory registers all components in Initialize()

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>
#include <string>

#include "../DataStructures/sfSlotMap.hpp"
#include "Types\sfIEntity.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  class ComponentFactory
  {
  public:
    static ComponentFactory* Instance(void);

    IEntity* CreateComponent(const std::string &name);
    template <class CompType>
    CompType* CreateComponent(void);

    void DeleteComponent(const std::string &name, const HNDL handle);
    void DeleteComponent(const IEntity *component);

  private:
    ComponentFactory(void);
    ~ComponentFactory(void);

    ComponentFactory(const ComponentFactory&) = delete;
    ComponentFactory& operator=(const ComponentFactory&) = delete;

    //Register all components
    void Initialize(void);

    template <typename CompType>
    void RegisterComponent(void);

    std::string _RemoveScope(const std::string name) const;

    static ComponentFactory *m_instance;

    std::unordered_map<std::string, ISlotMap*> m_compMap;
  };

  template <typename CompType>
  void ComponentFactory::RegisterComponent(void)
  {
    ISlotMap* newSlotMap = new SlotMap<CompType>();

    auto result =  m_compMap.insert(
      std::make_pair(_RemoveScope(typeid(CompType).name()), newSlotMap));

    SF_ASSERT(result.second, "Component is already registered");

    if (!result.second)
      delete newSlotMap;
  }

  template <class CompType>
  CompType* ComponentFactory::CreateComponent(void)
  {
    std::string compName = _RemoveScope(typeid(CompType).name());
    SF_ASSERT(m_compMap.find(compName) != m_compMap.end(),
      "Component is not registered");

    auto &map = m_compMap[compName];
    HNDL newHndl = map->Create();

    //Set component info
    CompType *newComp = static_cast<CompType*>(map->At(newHndl));
    newComp->m_hndl = newHndl;
    newComp->m_name = name;

    return newComp;
  }

#define SF_CREATE_COMP(Type) \
  return Sulfur::ComponentFactory::Instance()->CreateComponent<Type>()
}