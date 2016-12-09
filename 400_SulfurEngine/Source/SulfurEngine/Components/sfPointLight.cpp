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
#include "Components/sfTransform.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

namespace Sulfur
{

  PointLight::PointLight()
    : m_color(1.0f, 1.0f, 1.0f, 1.0f), m_range(5.0f), m_intensity(1.0f), m_castsShadows(false)
  {

  }

  void PointLight::Initialize(void)
  {

  }

  PointLight* PointLight::Clone(void) const
  {
    PointLight *newPointLight = SF_CREATE_COMP(PointLight);
    return newPointLight;
  }

  void PointLight::DrawDebug(DebugDraw *draw) const
  {
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(m_owner);
    draw->DrawSphere(transform->GetTranslation(), m_range, false, Vector4(1.0f, 1.0f, 0.0f, 1.0f));
  }

}
