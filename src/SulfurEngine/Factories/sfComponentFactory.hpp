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

#include "Reflection/sfReflection.hpp"
#include "../DataStructures/sfSlotMap.hpp"
#include "Types\sfIEntity.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  class ComponentFactory
  {
    SF_SINGLETON(ComponentFactory);

  public:
    typedef std::unordered_map<std::string, ISlotMap*> ComponentMap;
    class ComponentData;

    //Register all components
    void Initialize(void);

    bool IsRegistered(const std::string &compType) const;
    template <class CompType>
    bool IsRegistered(void) const;
    const std::vector<std::string>& GetComponentTypes() const;

    IEntity* CreateComponent(const std::string &name);
    template <class CompType>
    CompType* CreateComponent(void);

    void DeleteComponent(const std::string &name, const HNDL handle);
    void DeleteComponent(const IEntity *component);

    IEntity* GetComponent(const std::string &name, HNDL handle) const;
    template <class CompType>
    CompType* GetComponent(HNDL handle) const;

    ComponentData GetComponentData(const std::string &compName);
    template <class CompType>
    ComponentData GetComponentData(void);

    ComponentMap& GetComponentMap();

    void RegisterScriptType(const std::string &name);

  private:
    friend class ScriptManager;

    template <typename CompType>
    void _RegisterComponent(void);

    std::string _RemoveScope(const std::string name) const;

    std::vector<std::string> m_componentTypes;
    ComponentMap m_compMap;

    bool m_initialized;

  public:

    class ComponentData
    {
    public:
      ComponentData(ISlotMap *slotMap) : m_slotMap(slotMap) {}

      ISlotMap::Iterator begin(void)
      {
        return m_slotMap->begin();
      }

      ISlotMap::Iterator end(void)
      {
        return m_slotMap->end();
      }

    private:
      ISlotMap *m_slotMap;
    };
  };

  template <class CompType>
  bool ComponentFactory::IsRegistered(void) const
  {
    std::string compType = _RemoveScope(typeid(CompType).name());

    auto res = m_compMap.find(compType);

    if (res != m_compMap.end())
      return true;
    return false;
  }

  template <typename CompType>
  void ComponentFactory::_RegisterComponent(void)
  {
    ISlotMap* newSlotMap = new SlotMap<CompType>();

    auto result =  m_compMap.insert(
      std::make_pair(_RemoveScope(typeid(CompType).name()), newSlotMap));

    SF_ASSERT(result.second, "Component is already registered");

    if (!result.second)
      delete newSlotMap;

    m_componentTypes.push_back(_RemoveScope(typeid(CompType).name()));
  }

  template <class CompType>
  CompType* ComponentFactory::CreateComponent(void)
  {
    std::string compName = _RemoveScope(typeid(CompType).name());
    SF_ASSERT(m_compMap.find(compName) != m_compMap.end(),
      compName + " is not registered");

    auto &map = m_compMap[compName];
    HNDL newHndl = map->Create();

    //Set component info
    CompType *newComp = static_cast<CompType*>(map->At(newHndl));
    newComp->m_hndl = newHndl;
    newComp->m_name = compName;

    return newComp;
  }

  template <class CompType>
  CompType* ComponentFactory::GetComponent(const HNDL handle) const
  {
    SF_ASSERT(handle != SF_INV_HANDLE, "Invalid handle");

    std::string compName = _RemoveScope(typeid(CompType).name());
    SF_ASSERT(m_compMap.find(compName) != m_compMap.end(),
      compName + " is not registered");

    return static_cast<CompType*>(m_compMap.at(compName)->At(handle));
  }

  template <class CompType>
  ComponentFactory::ComponentData
    ComponentFactory::GetComponentData(void)
  {
    std::string compName = _RemoveScope(typeid(CompType).name());
    SF_ASSERT(m_compMap.find(compName) != m_compMap.end(),
      compName + " is not registered");

    return ComponentData(m_compMap[compName]);
  }

#define SF_CREATE_COMP(Type) \
Sulfur::ComponentFactory::Instance()->CreateComponent<Type>()

#define SF_GET_COMP_STR(TypeStr, Handle) \
Sulfur::ComponentFactory::Instance()->GetComponent(TypeStr, Handle)

#define SF_GET_COMP_TYPE(Type, Handle) \
Sulfur::ComponentFactory::Instance()->GetComponent<Type>(Handle)

#define SF_GET_COMP_DATA(Type) \
Sulfur::ComponentFactory::Instance()->GetComponentData<Type>()
}