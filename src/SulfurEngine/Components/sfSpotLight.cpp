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

void SpotLight::Initialize(void)
{

}

SpotLight* SpotLight::Clone(void) const
{
  SpotLight *newSpotLight = SF_CREATE_COMP(SpotLight);
  return newSpotLight;
}

}
