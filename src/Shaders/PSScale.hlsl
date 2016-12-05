#include "PixelInputs.hlsli"

Texture2D TEX_Frame : register(t0);
SamplerState SS_Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  return TEX_Frame.Sample(SS_Sampler, input.texCoords);
}
