#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "BRDF.hlsli"

static const uint SAMPLES = 20;

Texture2DArray gbuffer : register(t0);
TextureCube environmentMap : register(t1);
TextureCube irradianceMap : register(t2);
SamplerState pointSampler;

float Random(float2 uv)
{
  uint x = uint(uv.x);
  uint y = uint(uv.y);
  return float(30u * x ^ y + 10u * x * y);
}

float radicalInverse_VdC(uint bits) {
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
  return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(in uint i, in uint count)
{
  return float2(float(i) / float(count), radicalInverse_VdC(i));
}

float3 GgxSample(in float2 e, in float roughness)
{
  float a2 = pow4(roughness);
  float phi = 2.0f * 3.14159f * e.x;
  float cosTheta = sqrt((1.0f - e.y) / (1.0f + (a2 - 1.0f) * e.y));
  float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

  return float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}

float RadianceLOD(in uint sampleCount, in float NoH, in float VoH, in float roughness, in uint width, in uint height)
{
  float D = GgxDistribution(roughness * roughness, NoH) * NoH / (4.0f * VoH);
  return max(0.0f, 0.5f * (log2(float(width * height) / sampleCount) - log2(D)));
}

float3 Radiance(in TextureCube env, in SamplerState ss, in float3 specularColor, in float3 normal, in float3 viewDirection, in float roughness, in float2 uv)
{
  uint w, h;
  env.GetDimensions(w, h);

  float3 up = abs(normal.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);
  float3 tangentX = normalize(cross(up, normal));
  float3 tangentY = cross(normal, tangentX);

  float NoV = saturate(dot(normal, viewDirection));
  float3 final = float3(0.0f, 0.0f, 0.0f);
  for (uint i = 0; i < SAMPLES; ++i)
  {
    float2 e = Hammersley(i, SAMPLES);
    float3 sampleDirection = GgxSample(e, roughness);
    float3 H = normalize(sampleDirection.x * tangentX + sampleDirection.y * tangentY + sampleDirection.z * normal);
    float3 L = normalize(reflect(-viewDirection, H));

    float NoL = saturate(dot(normal, L));
    float NoH = saturate(dot(normal, H));
    float VoH = saturate(dot(viewDirection, H));

    if (NoL > 0.0f)
    {
      float lod = RadianceLOD(SAMPLES, NoH, VoH, roughness, w, h);

      float3 F = SchlickFresnel(VoH, specularColor);
      float G = CookTorrenceGeometry(NoL, NoV, NoH, VoH);
      float3 envColor = pow(abs(env.SampleLevel(ss, L, lod).rgb), 2.2f);

      final += F * G * envColor * VoH / (NoH * NoV);
    }
  }

  return final / float(SAMPLES);
}

float4 main(PixelIn input) : SV_TARGET
{
  GBufferData gbufferData;
  UnpackGBuffer(gbuffer, pointSampler, input.texCoords, gbufferData);
  if (gbufferData.Diffuse.a == 0.0f) discard;

  float L = normalize(-reflect(gbufferData.ViewDirection, gbufferData.Normal));
  float NoL = max(0.0f, dot(gbufferData.Normal, L));
  float NoV = max(0.0f, dot(gbufferData.Normal, gbufferData.ViewDirection));
  float3 specularColor = CalculateSpecularColor(gbufferData.Diffuse.rgb, gbufferData.Metallic);

  float3 irradiance = irradianceMap.Sample(pointSampler, gbufferData.Normal).rgb;
  float3 diffuse = irradiance * gbufferData.Diffuse.rgb / 3.14159f;
  float3 specular = Radiance(environmentMap, pointSampler, specularColor, gbufferData.Normal, gbufferData.ViewDirection, gbufferData.Roughness, input.texCoords);

  //return float4(specular, 1.0f);
  return float4(BlendDiffuseSpecular(diffuse, specular, gbufferData.Diffuse.rgb, gbufferData.Metallic) + gbufferData.Emissive.rgb, 1.0f);
}
