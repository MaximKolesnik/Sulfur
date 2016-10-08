#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

cbuffer PointLightData
{
  float4 LightPosition;
  float4 LightColor;
  float LightRange;
  float LightIntensity;
};

Texture2DArray gbuffer;
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(gbuffer, pointSampler, input.texCoords, gbufferData);
  if (gbufferData.Diffuse.a == 0.0f) discard;

  float3 lightDirection = LightPosition - gbufferData.WorldPosition;
  float lightDistance = length(lightDirection);
  lightDirection /= lightDistance;
  float3 halfVector = normalize(lightDirection + gbufferData.ViewDirection);

  float NoL = max(0.0f, dot(gbufferData.Normal, lightDirection));
  float NoH = max(0.0f, dot(gbufferData.Normal, halfVector));
  float NoV = max(0.0f, dot(gbufferData.Normal, gbufferData.ViewDirection));
  float LoV = max(0.0f, dot(lightDirection, gbufferData.ViewDirection));
  float LoH = max(0.0f, dot(lightDirection, halfVector));
  float VoH = max(0.0f, dot(gbufferData.ViewDirection, halfVector));

  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);
  float3 F = SchlickFresnel(VoH, specularColor);
  float G = NewmannGeometry(NoL, NoV);
  float D = GgxDistribution(pow2(gbufferData.Roughness), NoH);
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.00001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  float d = max(lightDistance - LightRange, 0);
  float denom = d / LightRange + 1.0f;
  float atten = 1.0f / pow2(denom);
  atten = (atten - 0.001f) / (1.0f - 0.001f);
  atten = max(0, atten);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * atten * NoL * LightIntensity * LightColor, 1.0f);
  //return float4((NoL * diffuseColor / 3.14159f + specular) * atten, 1.0f);
}
