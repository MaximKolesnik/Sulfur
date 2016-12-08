/******************************************************************************/
/*!
\par     Sulfur
\file    PSDeferredAmbient.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering ambient lighting.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "IBL.hlsli"

TextureCube TEX_EnvironmentCube;
TextureCube TEX_IrradianceCube;
Texture2D TEX_OcclusionMap;
SamplerState SS_Sampler;

cbuffer AmbientLightData
{
  matrix InverseView;
  float4 AmbientLight;
  bool UseIBL;
};

float4 main(ScreenPixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(SS_Sampler, input.texCoords, gbufferData);
  float occlusion = TEX_OcclusionMap.Sample(SS_Sampler, input.texCoords).r;

  if (!UseIBL) return float4(AmbientLight.rgb * AmbientLight.a * gbufferData.Diffuse * occlusion, 1.0f);

  float3 V = -mul(float4(normalize(gbufferData.ViewPosition), 0.0f), InverseView).xyz;
  float3 N = mul(float4(gbufferData.Normal, 0.0f), InverseView).xyz;
  float3 L = normalize(-reflect(V, N));
  float NoL = max(0.0f, dot(N, L));
  float NoV = max(0.0f, dot(N, V));
  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse, gbufferData.Metallic);

  float3 irradiance = GammaToLinear(TEX_IrradianceCube.Sample(SS_Sampler, N).rgb);
  float3 diffuse = irradiance * gbufferData.Diffuse / 3.14159f;
  float3 specular = Radiance(TEX_EnvironmentCube, SS_Sampler, specularColor, N, V, gbufferData.Roughness, input.texCoords);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse, gbufferData.Metallic) * occlusion, 1.0f);
}
