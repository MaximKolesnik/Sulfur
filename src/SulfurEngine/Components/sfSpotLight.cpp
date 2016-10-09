/******************************************************************************/
/*!
\par     Sulfur
\file    sfSpotLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Spot light component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSpotLight.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

  SpotLight::SpotLight()
    : m_color(1.0f, 1.0f, 1.0f, 1.0f), m_range(5.0f), m_intensity(1.0f), 
    m_innerAngle(35.0f), m_outerAngle(40.0f), m_falloff(1.0f), m_castsShadows(false)
  {

  }

  void SpotLight::Initialize(void)
  {

  }

  SpotLight* SpotLight::Clone(void) const
  {
    SpotLight *newSpotLight = SF_CREATE_COMP(SpotLight);
    return newSpotLight;
  }

}
