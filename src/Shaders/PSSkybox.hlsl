#include "PixelInputs.hlsli"

TextureCube skyboxMap : register(t0);
SamplerState pointSampler;

float4 main(SkyboxPixelIn input) : SV_TARGET
{
  return skyboxMap.Sample(pointSampler, input.texCoords);
}
