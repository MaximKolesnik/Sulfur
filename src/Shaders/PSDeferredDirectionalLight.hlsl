#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

cbuffer DirectionalLightData
{
  float3 LightDirection;
  float4 LightColor;
  float LightIntensity;
};

Texture2DArray gbuffer;
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(gbuffer, pointSampler, input.texCoords, gbufferData);
  if (gbufferData.Diffuse.a == 0.0f) discard;

  float3 lightDirection = -LightDirection;
  float3 halfVector = normalize(lightDirection + gbufferData.ViewDirection);

  float NoL = max(0.0f, dot(gbufferData.Normal, lightDirection));
  float NoH = max(0.0f, dot(gbufferData.Normal, halfVector));
  float NoV = max(0.0f, dot(gbufferData.Normal, gbufferData.ViewDirection));
  //float LoV = max(0.0f, dot(lightDirection, gbufferData.ViewDirection));
  //float LoH = max(0.0f, dot(lightDirection, halfVector));
  float VoH = max(0.0f, dot(gbufferData.ViewDirection, halfVector));

  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);
  float3 F = SchlickFresnel(VoH, specularColor);
  float G = NewmannGeometry(NoL, NoV);
  float D = GgxDistribution(pow2(gbufferData.Roughness), NoH);
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.00001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * NoL * LightIntensity * LightColor, 1.0f);
}
