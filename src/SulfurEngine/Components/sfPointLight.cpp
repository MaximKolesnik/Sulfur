/******************************************************************************/
/*!
\par     Sulfur
\file    sfPointLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Point light component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfPointLight.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

void PointLight::Initialize(void)
{

}

PointLight* PointLight::Clone(void) const
{
  PointLight *newPointLight = SF_CREATE_COMP(PointLight);
  return newPointLight;
}

}
