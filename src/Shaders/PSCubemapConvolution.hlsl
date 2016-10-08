#include "PixelInputs.hlsli"
#include "GBuffer.hlsli"

cbuffer ConvolutionData
{
  uint CubeFace;
  uint OutputWidth;
  uint OutputHeight;
  float PhiRange;
  float PhiStep;
  float ThetaRange;
  float ThetaStep;
};

TextureCube environmentMap;
SamplerState pointSampler;

float4 main(PixelIn input) : SV_TARGET
{
  input.position.xy = input.position.xy / float2(OutputWidth, OutputHeight) * 2.0f - 1.0f;
  input.position.y *= -1.0f;

  float3 normal = normalize(float3(input.position.xy, 1));
  if (CubeFace == 2)
    normal = normalize(float3(input.position.x,  1, -input.position.y));
  else if (CubeFace == 3)
    normal = normalize(float3(input.position.x, -1,  input.position.y));
  else if (CubeFace == 0)
    normal = normalize(float3(1, input.position.y,-input.position.x));
  else if (CubeFace == 1)
    normal = normalize(float3(-1, input.position.y, input.position.x));
  else if (CubeFace == 5)
    normal = normalize(float3(-input.position.x, input.position.y, -1));

  float3 up = float3(0,1,0);
  float3 right = normalize(cross(up,normal));
  up = cross(normal,right);

  float3 sampledColour = float3(0,0,0);
  float index = 0;
  for (float phi = 0; phi < PhiRange; phi += PhiStep)
  {
    for (float theta = 0; theta < ThetaRange; theta += ThetaStep)
    {
      float3 temp = cos(phi) * right + sin(phi) * up;
      float3 sampleVector = cos(theta) * normal + sin(theta) * temp;
      sampledColour += environmentMap.Sample(pointSampler, sampleVector).rgb *
        cos(theta) * sin(theta);
      index++;
    }
  }

  return float4(3.14159f * sampledColour / index, 1);
  //return float4(CubeFace / 5.0f, CubeFace / 5.0f, CubeFace / 5.0f, 1);
}
