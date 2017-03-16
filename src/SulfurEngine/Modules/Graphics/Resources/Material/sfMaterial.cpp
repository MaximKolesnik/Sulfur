/******************************************************************************/
/*!
\par     Sulfur
\file    sfMaterial.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Base material

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfMaterial.hpp"

namespace Sulfur
{

  const Real Material::c_minRoughness = 0.01f;
  
  Material::Material()
    : m_diffuseColor(1.0f, 1.0f, 1.0f, 1.0f), m_diffuseTextureResource(nullptr), m_diffuseTextureResourcePath(), m_diffuseTiling(1.0f, 1.0f),
    m_metallic(0.0f), m_roughness(c_minRoughness), m_materialTextureResource(nullptr), m_materialTextureResourcePath(), m_materialTiling(1.0f, 1.0f),
    m_normalTextureResource(nullptr), m_normalTextureResourcePath(), m_normalTiling(1.0f, 1.0f),
    m_emissiveColor(0.0f, 0.0f, 0.0f, 0.0f), m_emissiveTextureResource(nullptr), m_emissiveTextureResourcePath(), m_emissiveTiling(1.0f, 1.0f)
  {
  }

}
