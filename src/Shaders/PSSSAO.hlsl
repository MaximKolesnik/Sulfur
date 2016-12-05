#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"
#include "Common.hlsli"

Texture2D TEX_NoiseMap;
SamplerState SS_Sampler;

#define SAMPLE_COUNT 16
#define SPIRAL_COUNT 10

cbuffer OcclusionData
{
  float Radius;
  float Amount;
  float DepthThreshold;
  float Contrast;
  float MaxDistance;
  float Phase;
  float NearPlane;
  float FarPlane;
};

float2 tapLocation(int sampleNumber, int samples, float spinAngle)
{
  float alpha = (sampleNumber + 0.5f) / samples;
  float angle = alpha * (SPIRAL_COUNT * 6.28f) + spinAngle;

  float sin_v, cos_v;
  sincos(angle, sin_v, cos_v);
  return float2(cos_v, sin_v) * alpha;
}

float4 main(ScreenPixelIn input) : SV_TARGET
{
  float3 viewPos = UnpackGBufferViewPosition(SS_Sampler, input.texCoords);
  if (viewPos.z > MaxDistance) return float4(1.0f, 1.0f, 1.0f, 1.0f);

  float3 normal = UnpackGBufferNormal(SS_Sampler, input.texCoords);
  float perspectiveRadius = Radius / (2.0f * viewPos.z);
  float randomAngle = Random(input.position.xy) + Phase;
  float r2 = Radius * Radius;

  float occlusion = 0.0f;
  for (int i = 0; i < SAMPLE_COUNT; ++i)
  {
    float2 sampleOffset = tapLocation(i, SAMPLE_COUNT, randomAngle) * perspectiveRadius;
    float2 sampleCoords = input.texCoords + sampleOffset;

    float3 samplePos = UnpackGBufferViewPosition(SS_Sampler, sampleCoords);
    float3 v = samplePos - viewPos;
    float sqrLen = dot(v, v);
    float heaveside = step(sqrLen, r2);
    float dD = DepthThreshold * viewPos.z;

    float edgeError = step(0.0, sampleCoords.x) * step(0.0, 1.0 - sampleCoords.x) *
                      step(0.0, sampleCoords.y) * step(0.0, 1.0 - sampleCoords.y);
    occlusion += max(0.0f, ((dot(normal, v) + dD) * heaveside * edgeError) / (sqrLen + 0.001f));
  }

  occlusion = 2.0f * Amount / SAMPLE_COUNT * occlusion;
  occlusion = max(0.0f, 1.0f - occlusion);
  occlusion = pow(occlusion, Contrast);

  return float4(occlusion, occlusion, occlusion, 1.0f);
}
