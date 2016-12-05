#include "PixelInputs.hlsli"
#include "HDR.hlsli"
#include "Common.hlsli"

Texture2D TEX_Frame : register(t0);
Texture2D TEX_Luminance : register(t1);
Texture2D TEX_Bloom : register(t2);
SamplerState SS_PointSampler : register(s0);
SamplerState SS_LinearSampler : register(s1);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float averageLum = AverageLuminance(TEX_Luminance, SS_LinearSampler, input.texCoords);
  float3 color = ExposedColor(TEX_Frame.Sample(SS_PointSampler, input.texCoords).rgb, averageLum).rgb;
  color = TONEMAP(color) + TEX_Bloom.Sample(SS_LinearSampler, input.texCoords).rgb * 0.6f;
  color = LinearToGamma(color);

  return float4(Dither(color, input.position.xy), 1.0f);
}
