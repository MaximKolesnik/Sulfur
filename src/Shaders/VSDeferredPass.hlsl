#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

DeferredPixelIn main(VertexIn input)
{
  DeferredPixelIn output;

  output.position = mul(input.position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.viewPosition = output.position.xyz;
  output.position = mul(output.position, ProjMatrix);

  output.texCoords = input.texCoords.xy;

  output.normal = mul(float4(input.normal.xyz, 0.0f), WorldMatrix).xyz;
  output.normal = mul(float4(output.normal.xyz, 0.0f), ViewMatrix).xyz;
  output.tangent = mul(float4(input.tangent.xyz, 0.0f), WorldMatrix).xyz;
  output.tangent = mul(float4(output.tangent.xyz, 0.0f), ViewMatrix).xyz;
  output.binormal = mul(float4(input.binormal.xyz, 0.0f), WorldMatrix).xyz;
  output.binormal = mul(float4(output.binormal.xyz, 0.0f), ViewMatrix).xyz;

  output.color = input.color;

  return output;
}