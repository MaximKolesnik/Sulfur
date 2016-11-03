#include "VertexInputs.hlsli"

float4 main(VertexIn input) : SV_POSITION
{
  float4 position = mul(input.position, WorldMatrix);
  position = mul(position, ViewMatrix);
  position = mul(position, ProjMatrix);

  return position;
}