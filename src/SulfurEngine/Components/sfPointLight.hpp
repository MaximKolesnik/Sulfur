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
#pragma once
#include "Types/sfIEntity.hpp"
#include "Modules/Graphics/Types/sfColor.hpp"

namespace Sulfur
{
	
SF_REFLECTED_CLASS_DERIVED(PointLight, IEntity)

public:
  virtual void Initialize(void) override final;
  virtual PointLight* Clone(void) const override final;

private:
  SF_PRIVATE_PROPERTY(Real, range, Range, "Range")
  SF_PRIVATE_PROPERTY(Color, color, Color, "Color")
  SF_PRIVATE_PROPERTY(Real, intensity, Intensity, "Intensity")
  SF_PRIVATE_PROPERTY(bool, castsShadows, CastsShadows, "Casts Shadows")
  
};
  
}