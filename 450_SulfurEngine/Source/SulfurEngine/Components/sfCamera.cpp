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

Camera::Camera()
  : m_projectionType(ProjectionType::PERSPECTIVE), m_fieldOfView(45.0f), m_nearPlane(0.1f), m_farPlane(1000.0f)
{

}

void Camera::Initialize(void)
{

}

Camera* Camera::Clone(void) const
{
  Camera *newCamera = SF_CREATE_COMP(Camera);
  newCamera->m_projectionType = m_projectionType;
  newCamera->m_fieldOfView = m_fieldOfView;
  newCamera->m_nearPlane = m_nearPlane;
  newCamera->m_farPlane = m_farPlane;
  return newCamera;
}

}
