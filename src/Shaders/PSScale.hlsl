/******************************************************************************/
/*!
\par     Sulfur
\file    PSScale.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for down/upscaling a texture.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

Texture2D TEX_Frame : register(t0);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  return TEX_Frame.Sample(SS_Sampler, input.texCoords);
}
