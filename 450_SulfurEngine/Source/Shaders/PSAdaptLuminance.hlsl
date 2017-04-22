/******************************************************************************/
/*!
\par     Sulfur
\file    PSAdaptLuminance.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering adaptive luminance map.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "HDR.hlsli"

#define TONEMAP TonemapDragoLog

cbuffer AdaptLuminanceData
{
  float DeltaTime;
  float Rate;
};

Texture2D TEX_LastLuminance : register(t0);
Texture2D TEX_Luminance : register(t1);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float lastLum = exp(TEX_LastLuminance.Sample(SS_Sampler, input.texCoords).r);
  float currentLum = TEX_Luminance.Sample(SS_Sampler, input.texCoords).r;
  float adaptedLum = lastLum + (currentLum - lastLum) * (1.0f - exp(-DeltaTime * Rate));
  return float4(log(adaptedLum), 1.0f, 1.0f, 1.0f);
}
