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
};

Texture2DArray gbuffer : register(t0);
Texture2D shadowMap : register(t1);
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(gbuffer, pointSampler, input.texCoords, gbufferData);
  if (gbufferData.Diffuse.a == 0.0f) discard;

  // Check shadow map
  float shadowFactor = 0.0f;
  float4 shadowPos = mul(float4(gbufferData.WorldPosition, 1.0f), ShadowMapTransform);
  shadowPos.xy = shadowPos.xy / shadowPos.w / 2.0f + 0.5f;
  if (shadowPos.x >= 0.0f && shadowPos.x <= 1.0f &&  shadowPos.y >= 0.0f && shadowPos.y <= 1.0f)
    if (shadowPos.x >= 0.0f && shadowPos.x <= 1.0f &&  shadowPos.y >= 0.0f && shadowPos.y <= 1.0f)
    {
      float smDepth = shadowMap.Sample(pointSampler, float2(shadowPos.x, 1.0f - shadowPos.y)).r;

      float3 lightDirection = gbufferData.WorldPosition - LightPosition;
      float depth = dot(lightDirection, lightDirection) - 0.001f;

      if (depth - 0.001f < smDepth)
        shadowFactor = 1.0f;
    }

  if (shadowFactor == 0.0f) discard;

  float3 lightDirection = -LightDirection;
  float3 halfVector = normalize(lightDirection + gbufferData.ViewDirection);

  float NoL = max(0.0f, dot(gbufferData.Normal, lightDirection));
  float NoH = max(0.0f, dot(gbufferData.Normal, halfVector));
  float NoV = max(0.0f, dot(gbufferData.Normal, gbufferData.ViewDirection));
  float VoH = max(0.0f, dot(gbufferData.ViewDirection, halfVector));

  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);
  float3 F = SchlickFresnel(VoH, specularColor);
  float G = NewmannGeometry(NoL, NoV);
  float D = GgxDistribution(pow2(gbufferData.Roughness), NoH);
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.00001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * NoL * LightIntensity * LightColor * shadowFactor, 1.0f);
}
