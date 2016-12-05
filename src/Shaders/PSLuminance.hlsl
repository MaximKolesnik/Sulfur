#include "PixelInputs.hlsli"
#include "HDR.hlsli"

Texture2D TEX_Frame : register(t0);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float luminance = Luminance(TEX_Frame.Sample(SS_Sampler, input.texCoords).rgb);
  return float4(luminance, 1.0f, 1.0f, 1.0f);
}
