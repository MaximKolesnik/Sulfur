/******************************************************************************/
/*!
\par     Sulfur
\file    sfDirectionalLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Directional light component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDirectionalLight.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

  DirectionalLight::DirectionalLight()
    : m_color(1.0f, 1.0f, 1.0f, 1.0f), m_intensity(1.0f), m_castsShadows(false)
  {
  }

  void DirectionalLight::Initialize(void)
  {

  }

  DirectionalLight* DirectionalLight::Clone(void) const
  {
    DirectionalLight *newDirectionalLight = SF_CREATE_COMP(DirectionalLight);
    return newDirectionalLight;
  }

}
