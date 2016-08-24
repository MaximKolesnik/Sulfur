#include "sfComponentFactory.hpp"
#include "../Components/sfTransform.hpp"

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

  void ComponentFactory::Initialize(void)
  {
    this->RegisterComponent<Transform>();
  }

  ComponentFactory::ComponentFactory(void)
  {

  }

  ComponentFactory::~ComponentFactory(void)
  {

  }
}