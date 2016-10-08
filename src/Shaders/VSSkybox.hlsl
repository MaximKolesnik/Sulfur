#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

SkyboxPixelIn main(VertexIn input)
{
  SkyboxPixelIn output;

  output.position = mul(input.position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.position = mul(output.position, ProjMatrix);

  output.texCoords = normalize(input.position.xyz);

  return output;
}