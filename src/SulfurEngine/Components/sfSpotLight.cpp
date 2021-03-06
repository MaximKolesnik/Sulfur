/******************************************************************************/
/*!
\par     Sulfur
\file    sfSpotLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Spot light component

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSpotLight.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

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
    newSpotLight->m_color = m_color;
    newSpotLight->m_range = m_range;
    newSpotLight->m_intensity = m_intensity;
    newSpotLight->m_innerAngle = m_innerAngle;
    newSpotLight->m_outerAngle = m_outerAngle;
    newSpotLight->m_falloff = m_falloff;
    newSpotLight->m_castsShadows = m_castsShadows;
    return newSpotLight;
  }

  void SpotLight::DrawDebug(DebugDraw *draw) const
  {
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(m_owner);
    transform->Update();
    draw->DrawCone(transform->GetTranslation(), transform->GetWorldForward(), transform->GetWorldRight(), transform->GetWorldUp(), m_range, m_outerAngle, false, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  }

}
