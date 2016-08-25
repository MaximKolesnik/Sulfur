/******************************************************************************/
/*!
\par     Sulfur
\file    sfComponentFactory.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Component Factory registers all components in Initialize()

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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