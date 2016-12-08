/******************************************************************************/
/*!
\par     Sulfur
\file    PSScaleCubemap.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for down/upscaling a cubemap.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

cbuffer CubemapScaleData
{
  uint CubeFace;
};

TextureCube TEX_Cubemap;
SamplerState SS_Sampler;

float4 main(ScreenPixelIn input) : SV_TARGET
{
  input.position.xy = input.texCoords * 2.0f - 1.0f;
  input.position.y *= -1.0f;

  float3 normal = normalize(float3(input.position.xy, 1));
  if (CubeFace == 2)
    normal = normalize(float3(input.position.x,  1, -input.position.y));
  else if (CubeFace == 3)
    normal = normalize(float3(input.position.x, -1,  input.position.y));
  else if (CubeFace == 0)
    normal = normalize(float3(1, input.position.y,-input.position.x));
  else if (CubeFace == 1)
    normal = normalize(float3(-1, input.position.y, input.position.x));
  else if (CubeFace == 5)
    normal = normalize(float3(-input.position.x, input.position.y, -1));

  return TEX_Cubemap.Sample(SS_Sampler, normal);
}
