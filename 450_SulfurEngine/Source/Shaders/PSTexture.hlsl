/******************************************************************************/
/*!
\par     Sulfur
\file    PSTexture.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Simple texture pixel shader.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

Texture2D diffuseTexture;
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  return diffuseTexture.Sample(pointSampler, 1.0f - input.texCoords);
}
