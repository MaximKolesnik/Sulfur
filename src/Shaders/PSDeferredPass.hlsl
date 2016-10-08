#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D emissiveTexture : register(t3);
SamplerState pointSampler : register(s0);

GBufferOutput main(DeferredPixelIn input)
{
  GBufferOutput output;

  output.Diffuse = float4(MaterialDiffuseColor.rgb, 1.0f);
  if (MaterialUsesDiffuseTexture) output.Diffuse.rgb *= diffuseTexture.Sample(pointSampler, 1.0f - input.texCoords).rgb;

  output.NormalMetallic = float4(normalize(input.normal), MaterialMetallic);
  output.WorldPositionRoughness = float4(input.worldPosition, MaterialRoughness);
  if (MaterialUsesMaterialTexture)
  {
    float4 materialData = materialTexture.Sample(pointSampler, 1.0f - input.texCoords);
    output.NormalMetallic.a *= materialData.r;
    output.WorldPositionRoughness.a *= materialData.g;
  }

  if (MaterialUsesNormalTexture)
  {
    float3 normalMap = normalTexture.Sample(pointSampler, 1.0f - input.texCoords).xyz * 2.0f - 1.0f;
    float3x3 tbn = float3x3(normalize(input.tangent), normalize(input.binormal), output.NormalMetallic.xyz);
    output.NormalMetallic.rgb = mul(normalMap, tbn);
  }

  output.ViewSpacePositionDepth = float4(input.viewSpacePosition, 0.0f);
  output.ViewDirection = float4(normalize(input.viewDirection), 0.0f);

  output.Emissive = float4(MaterialEmissiveColor.rgb * MaterialEmissiveColor.a, 1.0f);
  if (MaterialUsesEmissiveTexture)
  {
    float4 emissive = emissiveTexture.Sample(pointSampler, 1.0f - input.texCoords);
    output.Emissive.rgb *= (emissive.rgb * emissive.a);
  }

  return output;
}
