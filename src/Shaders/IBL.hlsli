#include "BRDF.hlsli"
#include "Common.hlsli"

static const uint SAMPLES = 20;

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
      float3 envColor = pow(abs(GammaToLinear(env.SampleLevel(ss, L, lod).rgb)), 2.2f);

      final += F * G * envColor * VoH / (NoH * NoV + 0.00001f);
    }
  }

  return final / float(SAMPLES);
}
