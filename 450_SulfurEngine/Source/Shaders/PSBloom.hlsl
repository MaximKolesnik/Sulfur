/******************************************************************************/
/*!
\par     Sulfur
\file    PSBloom.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering bloom map.

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "HDR.hlsli"

Texture2D TEX_Frame : register(t0);
Texture2D TEX_Luminance : register(t1);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float3 color = TEX_Frame.Sample(SS_Sampler, input.texCoords).rgb;
  float averageLum = AverageLuminance(TEX_Luminance, SS_Sampler, input.texCoords);
  color = ExposedColor(color, averageLum).rgb;
  return float4(TONEMAP(color), 1.0f);
}
