/******************************************************************************/
/*!
\par     Sulfur
\file    sfMaterial.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Base material

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Reflection/sfReflection.hpp"
#include "Modules/Graphics/Types/sfColor.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"

namespace Sulfur
{
	
  SF_REFLECTED_CLASS(Material)

  public:
    Material();

    SF_PRIVATE_PROPERTY(Color, diffuseColor, DiffuseColor, "Diffuse Color")
    SF_RESOURCE(Texture2D, diffuseTexture, DiffuseTexture, "Diffuse Texture")
    SF_PRIVATE_PROPERTY_RANGE(Real, metallic, Metallic, "Metallic", 0.2f, 0.45f)
    SF_PRIVATE_PROPERTY_RANGE(Real, roughness, Roughness, "Roughness", c_minRoughness, 1.0f)
    SF_RESOURCE(Texture2D, materialTexture, MaterialTexture, "Material Texture")
    SF_RESOURCE(Texture2D, normalTexture, NormalTexture, "Normal Texture")
    SF_PRIVATE_PROPERTY(Color, emissiveColor, EmissiveColor, "Emissive Color")
    SF_RESOURCE(Texture2D, emissiveTexture, EmissiveTexture, "Emissive Texture")

    static const Real c_minRoughness;
  
  };
  
}