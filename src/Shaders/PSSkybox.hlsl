#include "PixelInputs.hlsli"
#include "Common.hlsli"

TextureCube TEX_Skybox : register(t0);
SamplerState SS_Sampler;

float4 main(SkyboxPixelIn input) : SV_TARGET
{
  return float4(GammaToLinear(TEX_Skybox.Sample(SS_Sampler, input.texCoords).rgb), 1.0f);
}
