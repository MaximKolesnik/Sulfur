#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

PixelIn main(VertexIn input)
{
  PixelIn output;

  output.position = mul(input.position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.position = mul(output.position, ProjMatrix);

  output.texCoords = input.texCoords.xy;
  output.color = input.color;

  return output;
}