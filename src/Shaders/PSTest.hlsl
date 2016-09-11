#include "PixelInputs.hlsli"

Texture2D diffuseTexture : register(t0);
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  return input.color;
}
