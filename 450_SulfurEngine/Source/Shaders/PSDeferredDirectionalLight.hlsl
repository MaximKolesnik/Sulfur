/******************************************************************************/
/*!
\par     Sulfur
\file    PSDeferredDirectionalLight.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering light map for directional lights.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

cbuffer DirectionalLightData
{
  matrix ShadowMapTransform;
  float3 LightPosition;
  float3 LightDirection;
  float4 LightColor;
  float LightIntensity;
  bool CastsShadows;
};

Texture2D TEX_ShadowMap;
SamplerState SS_Sampler;

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(SS_Sampler, input.texCoords, gbufferData);

  // Check shadow map
  float shadowFactor = 1.0f;

  if (CastsShadows)
  {
    float4 shadowPos = mul(float4(gbufferData.ViewPosition, 1.0f), ShadowMapTransform);
    shadowPos.xy = shadowPos.xy / shadowPos.w / 2.0f + 0.5f;
    if (shadowPos.x >= 0.0f && shadowPos.x <= 1.0f &&  shadowPos.y >= 0.0f && shadowPos.y <= 1.0f)
    {
      float smDepth = TEX_ShadowMap.Sample(SS_Sampler, float2(shadowPos.x, 1.0f - shadowPos.y)).r;

      float3 lightDirection = gbufferData.ViewPosition - LightPosition;
      float depth = dot(lightDirection, lightDirection) - 0.001f;

      if (depth - 0.001f >= smDepth)
        shadowFactor = 0.0f;
    }
  }

  if (shadowFactor == 0.0f) discard;

  float3 V = -normalize(gbufferData.ViewPosition);
  float3 L = -LightDirection;
  float3 H = normalize(L + V);

  float NoL = max(0.0f, dot(gbufferData.Normal, L));
  float NoH = max(0.0f, dot(gbufferData.Normal, H));
  float NoV = max(0.0f, dot(gbufferData.Normal, V));
  float VoH = max(0.0f, dot(V, H));

  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);
  float3 F = SchlickFresnel(VoH, specularColor);
  float G = NewmannGeometry(NoL, NoV);
  float D = GgxDistribution(pow2(gbufferData.Roughness), NoH);
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.00001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * NoL * LightIntensity * LightColor * shadowFactor, 1.0f);
}
