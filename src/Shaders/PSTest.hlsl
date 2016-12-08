/******************************************************************************/
/*!
\par     Sulfur
\file    PSTest.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Test pixel shader.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

Texture2D diffuseTexture : register(t0);
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  return input.color;
}
