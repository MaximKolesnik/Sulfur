/******************************************************************************/
/*!
\par     Sulfur
\file    PSDeferredSpotLight.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering light map for spot lights.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

cbuffer SpotLightData
{
  matrix ShadowMapTransform;
  float4 LightPosition;
  float3 LightDirection;
  float4 LightColor;
  float LightRange;
  float LightIntensity;
  float LightInnerAngle;
  float LightOuterAngle;
  float LightFalloff;
  bool CastsShadows;
};

Texture2D TEX_ShadowMap;
SamplerState SS_Sampler;

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(SS_Sampler, input.texCoords, gbufferData);

  float3 V = -normalize(gbufferData.ViewPosition);
  float3 viewPos = gbufferData.ViewPosition;
  float3 L = LightPosition.xyz - viewPos;
  float lightDistSqr = dot(L, L);
  float lightDistance = sqrt(lightDistSqr);
  if (lightDistance > LightRange || lightDistance <= 0.0f) discard;
  L /= lightDistance;
  float3 H = normalize(L + V);

  // Check if in angle range
  float spotPower = (dot(-LightDirection, L) - cos(LightOuterAngle)) / (cos(LightInnerAngle) - cos(LightOuterAngle));
  spotPower = pow(saturate(spotPower), LightFalloff);
  if (spotPower <= 0.0f) discard;

  // Check shadow map
  float shadowFactor = 1.0f;

  if (CastsShadows)
  {
    float4 shadowPos = mul(float4(viewPos, 1.0f), ShadowMapTransform);
    shadowPos.xy = shadowPos.xy / shadowPos.w / 2.0f + 0.5f;
    if (shadowPos.x >= 0.0f && shadowPos.x <= 1.0f &&  shadowPos.y >= 0.0f && shadowPos.y <= 1.0f)
    {
      float smDepth = TEX_ShadowMap.Sample(SS_Sampler, float2(shadowPos.x, 1.0f - shadowPos.y)).r;
      if (lightDistSqr - 0.001f >= smDepth)
        shadowFactor = 0.0f;
    }
  }

  float NoL = max(0.0f, dot(gbufferData.Normal, L));
  float NoH = max(0.0f, dot(gbufferData.Normal, H));
  float NoV = max(0.0f, dot(gbufferData.Normal, V));
  float LoV = max(0.0f, dot(L, V));
  float LoH = max(0.0f, dot(L, H));
  float VoH = max(0.0f, dot(V, H));

  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);
  float3 F = SchlickFresnel(VoH, specularColor);
  float G = NewmannGeometry(NoL, NoV);
  float D = GgxDistribution(pow2(gbufferData.Roughness), NoH);
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.00001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  // Attenuation
  float d = max(lightDistance - LightRange, 0);
  float denom = d / LightRange + 1.0f;
  float atten = 1.0f / pow2(denom);
  atten = (atten - 0.001f) / (1.0f - 0.001f);
  atten = max(0, atten);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * atten * NoL * LightIntensity * LightColor * spotPower * shadowFactor, 1.0f);
}
