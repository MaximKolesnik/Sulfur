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
#include "../Error/sfError.hpp"

#define TO_STRING(x) #x

namespace Sulfur
{
  class ComponentFactory
  {
  public:
    static ComponentFactory* Instance(void);

  private:
    ComponentFactory(void);
    ~ComponentFactory(void);

    ComponentFactory(const ComponentFactory&) = delete;
    ComponentFactory& operator=(const ComponentFactory&) = delete;

    //Register all components
    void Initialize(void);

    template <typename CompType>
    void RegisterComponent(void);

    static ComponentFactory *m_instance;

    std::unordered_map<std::string, ISlotMap*> m_compMap;
  };

  template <typename CompType>
  void ComponentFactory::RegisterComponent(void)
  {
    ISlotMap* newSlotMap = new SlotMap<CompType>();

    auto result =  m_compMap.insert(
      std::make_pair(TO_STRING(CompType), newSlotMap));

    SF_ASSERT(result.second, "Component is already registered");

    if (!result.second)
      delete newSlotMap;
  }

}