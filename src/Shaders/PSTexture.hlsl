#include "PixelInputs.hlsli"

Texture2D diffuseTexture;
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  return diffuseTexture.Sample(pointSampler, 1.0f - input.texCoords);
}
