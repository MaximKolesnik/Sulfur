/******************************************************************************/
/*!
\par     Sulfur
\file    sfDirectionalLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Directional light component

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDirectionalLight.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

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
    newDirectionalLight->m_color = m_color;
    newDirectionalLight->m_intensity = m_intensity;
    newDirectionalLight->m_castsShadows = m_castsShadows;
    return newDirectionalLight;
  }

  void DirectionalLight::DrawDebug(DebugDraw *draw) const
  {
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(m_owner);
    transform->Update();
    draw->DrawBox2D(transform->GetWorldMatrix(), 0.75f, 0.75f, false, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
    draw->DrawVector(transform->GetTranslation(), transform->GetForward() * 0.75f, false, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  }

}
