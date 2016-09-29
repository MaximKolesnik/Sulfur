/******************************************************************************/
/*!
\par     Sulfur
\file    sfCamera.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/16/2016

\brief   Camera component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfCamera.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

SF_BASE_TYPE_SERIALIZATION_DEFAULT(ProjectionType)

void Camera::Initialize(void)
{

}

Camera* Camera::Clone(void) const
{
  Camera *newCamera = SF_CREATE_COMP(Camera);
  return newCamera;
}

}
