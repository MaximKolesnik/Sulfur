/******************************************************************************/
/*!
\par     Sulfur
\file    sfComponentFactory.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Component Factory registers all components in Initialize()

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfComponentFactory.hpp"
#include "../Components/sfTransform.hpp"
#include "Types\sfObject.hpp"
#include "Factories\sfObjectFactory.hpp"

namespace Sulfur
{
  ComponentFactory *ComponentFactory::m_instance = ComponentFactory::Instance();

  ComponentFactory* ComponentFactory::Instance(void)
  {
    if (!m_instance)
    {
      m_instance = new ComponentFactory();
      m_instance->Initialize();
    }

    return m_instance;
  }

  bool ComponentFactory::IsRegistered(const std::string &compType) const
  {
    auto res = m_compMap.find(compType);

    if (res != m_compMap.end())
      return true;
    return false;
  }

  IEntity* ComponentFactory::CreateComponent(const std::string &name)
  {
    SF_ASSERT(m_compMap.find(name) != m_compMap.end(),
      "Component is not registered");

    auto &map = m_compMap[name];
    HNDL newHndl = map->Create();

    //Set component info
    IEntity *newComp = map->At(newHndl);
    newComp->m_hndl = newHndl;
    newComp->m_name = name;

    return newComp;
  }

  void ComponentFactory::DeleteComponent(const std::string &name, const HNDL handle)
  {
    SF_ASSERT(m_compMap.find(name) != m_compMap.end(), name + " component is not registered");
    SF_ASSERT(handle != SF_INV_HANDLE, "Invalid handle");

    Object *obj = SF_GET_OBJECT(m_compMap[name]->At(handle)->m_owner);

    SF_ASSERT(obj->m_components.find(name) != obj->m_components.end(), 
      name + " component is not attadched");
    SF_ASSERT(obj->m_components[name] == handle, name + " component handle does not match");

    obj->m_components.erase(name);
    m_compMap[name]->Erase(handle);
  }

  void ComponentFactory::DeleteComponent(const IEntity *component)
  {
    HNDL handle = component->m_hndl;
    const std::string &name = component->m_name;

    SF_ASSERT(m_compMap.find(name) != m_compMap.end(), name + " component is not registered");
    SF_ASSERT(handle != SF_INV_HANDLE, "Invalid handle");

    Object *obj = SF_GET_OBJECT(m_compMap[name]->At(handle)->m_owner);

    SF_ASSERT(obj->m_components.find(name) != obj->m_components.end(),
      name + " component is not attadched");
    SF_ASSERT(obj->m_components[name] == handle, name + " component handle does not match");

    obj->m_components.erase(name);
    m_compMap[name]->Erase(handle);
  }

  void ComponentFactory::Initialize(void)
  {
    this->_RegisterComponent<Transform>();
  }

  ComponentFactory::ComponentFactory(void)
  {

  }

  ComponentFactory::~ComponentFactory(void)
  {
    for (auto &it : m_compMap)
      delete it.second;
  }

  IEntity* ComponentFactory::GetComponent(const std::string &name, 
    HNDL handle) const
  {
    SF_ASSERT(handle != SF_INV_HANDLE, "Invalid handle");
    SF_ASSERT(m_compMap.find(name) != m_compMap.end(),
      name + " is not registered");

    return m_compMap.at(name)->At(handle);
  }

  ComponentFactory::ComponentData 
    ComponentFactory::GetComponentData(const std::string &compName)
  {
    SF_ASSERT(m_compMap.find(compName) != m_compMap.end(),
      compName + " is not registered");

    return ComponentData(m_compMap[compName]);
  }

  std::string ComponentFactory::_RemoveScope(const std::string name) const
  {
    size_t scopePos = name.find_last_of(":");
    if (scopePos == std::string::npos)
      return name;
    else
      return name.substr(scopePos + 1);
  }
}