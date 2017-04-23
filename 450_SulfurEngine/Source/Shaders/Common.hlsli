/******************************************************************************/
/*!
\par     Sulfur
\file    Common.hlsli
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Common functions and constants.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Constants.hlsli"

float Random(float2 uv)
{
  uint x = uint(uv.x);
  uint y = uint(uv.y);
  return float(30u * x ^ y + 10u * x * y);
}

float radicalInverse_VdC(uint bits) {
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
  return float(bits) * 2.3283064365386963e-10;
}

float2 Hammersley(in uint i, in uint count)
{
  return float2(float(i) / float(count), radicalInverse_VdC(i));
}

float3 GammaToLinear(float3 color)
{
  return pow(color, Gamma);
}

float3 LinearToGamma(float3 color)
{
  return pow(color, InvGamma);
}

float3 Dither(float3 color, float2 offset)
{
  float ditherBit = 8.0f;
  float gridPos = frac(dot(offset - float2(0.5f, 0.5f), float2(1.0f / 16.0f, 10.0f / 36.0f) + 0.25f));
  float ditherShift = 0.25f * (1.0f / (pow(2.0, ditherBit) - 1.0f));
  float3 ditherShiftRGB = float3(ditherShift, -ditherShift, ditherShift);
  ditherShiftRGB = lerp(2.0f * ditherShiftRGB, -2.0f * ditherShiftRGB, gridPos);
  return color + ditherShiftRGB;
}