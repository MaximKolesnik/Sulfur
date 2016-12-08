/******************************************************************************/
/*!
\par     Sulfur
\file    PSLuminance.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief  Pixel shader for rendering luminance map.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "HDR.hlsli"

Texture2D TEX_Frame : register(t0);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float luminance = Luminance(TEX_Frame.Sample(SS_Sampler, input.texCoords).rgb);
  return float4(luminance, 1.0f, 1.0f, 1.0f);
}
