#include "PixelInputs.hlsli"

PixelIn main(uint vertexId : SV_VERTEXID)
{
  PixelIn output;

  output.texCoords = float2(vertexId & 1, vertexId >> 1);
  output.position = float4((output.texCoords.x - 0.5f) * 2, -(output.texCoords.y - 0.5f) * 2, 0, 1);

  return output;
}
