/******************************************************************************/
/*!
\par     Sulfur
\file    PSDeferredPass.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering gbuffer.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "Common.hlsli"

Texture2D TEX_Diffuse;
Texture2D TEX_Normal;
Texture2D TEX_Material;
Texture2D TEX_Emissive;
SamplerState SS_Sampler;

GBufferOutput main(DeferredPixelIn input)
{
  GBufferOutput output;

  output.DiffuseMetallic.rgb = MaterialDiffuseColor.rgb;
  if (MaterialUsesDiffuseTexture) output.DiffuseMetallic.rgb *= GammaToLinear(TEX_Diffuse.Sample(SS_Sampler, 1.0f - (input.texCoords * MaterialDiffuseMaterialScaling.xy)).rgb);

  output.DiffuseMetallic.a = MaterialMetallic;

  output.NormalRoughness.xyz = normalize(input.normal);
  if (MaterialUsesNormalTexture)
  {
    float3 normalMap = TEX_Normal.Sample(SS_Sampler, 1.0f - (input.texCoords * MaterialNormalEmissiveScaling.xy)).xyz * 2.0f - 1.0f;
    float3x3 tbn = float3x3(normalize(input.tangent), normalize(input.binormal), output.NormalRoughness.xyz);
    output.NormalRoughness.xyz = mul(normalMap, tbn);
  }

  output.NormalRoughness.a = MaterialRoughness;

  output.ViewPosition.xyz = input.viewPosition;
  output.ViewPosition.a = 0.0f;

  return output;
}
