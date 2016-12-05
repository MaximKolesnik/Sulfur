#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

cbuffer PointLightData
{
  float4 LightPosition;
  float4 LightColor;
  float LightRange;
  float LightIntensity;
  bool CastsShadows;
};

TextureCube TEX_ShadowMap;
SamplerState SS_Sampler;

float4 main(ScreenPixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(SS_Sampler, input.texCoords, gbufferData);

  float3 V = -normalize(gbufferData.ViewPosition);
  float3 viewPos = gbufferData.ViewPosition;
  float3 L = LightPosition.xyz - viewPos;
  float lightDistSqr = dot(L, L);
  float lightDistance = sqrt(lightDistSqr);
  if (lightDistance > LightRange) discard;
  L /= lightDistance;
  float3 H = normalize(L + V);

  float shadowFactor = 1.0f;

  if (CastsShadows)
  {
    float smDepth = TEX_ShadowMap.Sample(SS_Sampler, -L).r;

    if (lightDistSqr - 0.001f >= smDepth)
      shadowFactor = 0.0f;
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
  float3 specular = F * G * D / max(4.0f * NoV * NoL, 0.0001f);
  float3 diffuse = gbufferData.Diffuse.rgb / 3.14159f * (1.0f- F);

  float d = max(lightDistance - LightRange, 0.0f);
  float denom = d / LightRange + 1.0f;
  float atten = 1.0f / pow2(denom);
  atten = (atten - 0.001f) / (1.0f - 0.001f);
  atten = max(0, atten);

  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) * atten * NoL * LightIntensity * LightColor.rgb * LightColor.a * shadowFactor, 1.0f);
  //return float4((NoL * diffuseColor / 3.14159f + specular) * atten, 1.0f);
}
