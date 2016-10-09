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
  virtual void Initialize(void) override final;
  virtual SpotLight* Clone(void) const override final;
  virtual void Update(void) override {}

private:
  SF_PRIVATE_PROPERTY(Real, range, Range, "Range")
  SF_PRIVATE_PROPERTY(Real, innerAngle, InnerAngle, "Inner Angle")
  SF_PRIVATE_PROPERTY(Real, outerAngle, OuterAngle, "Outer Angle")
  SF_PRIVATE_PROPERTY(Real, falloff, Falloff, "Falloff")
  SF_PRIVATE_PROPERTY(Color, color, Color, "Color")
  SF_PRIVATE_PROPERTY(Real, intensity, Intensity, "Intensity")
  SF_PRIVATE_PROPERTY(bool, castsShadows, CastsShadows, "Casts Shadows")
  
};
  
}