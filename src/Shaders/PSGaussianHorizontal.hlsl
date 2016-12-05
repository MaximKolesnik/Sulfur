#include "PixelInputs.hlsli"
#include "Blur.hlsli"

Texture2D InputTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(ScreenPixelIn input) : SV_TARGET
{
  uint width, height;
  InputTexture.GetDimensions(width, height);

  float4 color = InputTexture.Sample(Sampler, input.texCoords) * GaussianKernel[0];
  for (int i = 1; i < 3; ++i)
  {
    color += InputTexture.Sample(Sampler, (input.texCoords + float2(GaussianOffset[i] / width, 0.0f))) * GaussianKernel[i];
    color += InputTexture.Sample(Sampler, (input.texCoords - float2(GaussianOffset[i] / width, 0.0f))) * GaussianKernel[i];
  }

  return color;
}
