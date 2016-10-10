/******************************************************************************/
/*!
\par     Sulfur
\file    sfSpotLight.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   Spot light component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfIEntity.hpp"
#include "Modules/Graphics/Types/sfColor.hpp"

namespace Sulfur
{
	
SF_REFLECTED_CLASS_DERIVED(SpotLight, IEntity)

public:
  SpotLight();

  virtual void Initialize(void) override final;
  virtual SpotLight* Clone(void) const override final;
  virtual void Update(void) override {}
  virtual void DrawDebug(DebugDraw *draw) const override;

private:
  SF_PRIVATE_PROPERTY(Color, color, Color, "Color")
  SF_PRIVATE_PROPERTY(Real, range, Range, "Range")
  SF_PRIVATE_PROPERTY(Real, intensity, Intensity, "Intensity")
  SF_PRIVATE_PROPERTY_RANGE(Real, innerAngle, InnerAngle, "Inner Angle", 0.0f, 180.0f)
  SF_PRIVATE_PROPERTY_RANGE(Real, outerAngle, OuterAngle, "Outer Angle", 0.0f, 180.0f)
  SF_PRIVATE_PROPERTY(Real, falloff, Falloff, "Falloff")
  SF_PRIVATE_PROPERTY(bool, castsShadows, CastsShadows, "Casts Shadows")
  
};
  
}