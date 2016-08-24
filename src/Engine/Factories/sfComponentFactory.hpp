#pragma once

#include <unordered_map>
#include <string>

#include <assert.h>

#include "../DataStructures/sfSlotMap.hpp"

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

    assert(result.second && "Component is already registered");

    if (!result.second)
      delete newSlotMap;
  }

}