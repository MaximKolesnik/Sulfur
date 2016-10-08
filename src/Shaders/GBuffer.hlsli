struct GBufferOutput
{
  float4 Diffuse : SV_Target0;
  float4 NormalMetallic : SV_Target1;
  float4 WorldPositionRoughness : SV_Target2;
  float4 ViewSpacePositionDepth : SV_Target3;
  float4 ViewDirection : SV_Target4;
  float4 Emissive : SV_Target5;
};

struct GBufferData
{
  float4 Diffuse;
  float4 Emissive;
  float3 Normal;
  float3 WorldPosition;
  float3 ViewSpacePosition;
  float3 ViewDirection;
  float Depth;
  float Metallic;
  float Roughness;
};

void UnpackGBuffer(in Texture2DArray tex, in SamplerState ss, in float2 uv, out GBufferData data)
{
  float4 diffuse = tex.Sample(ss, float3(uv, 0));
  float4 normalMetallic = tex.Sample(ss, float3(uv, 1));
  float4 worldPositionRoughness = tex.Sample(ss, float3(uv, 2));
  float4 viewSpacePositionDepth = tex.Sample(ss, float3(uv, 3));
  float4 viewDirection = tex.Sample(ss, float3(uv, 4));
  float4 emissive = tex.Sample(ss, float3(uv, 5));

  data.Diffuse = diffuse;
  data.Emissive = emissive;
  data.Normal = normalMetallic.xyz;
  data.WorldPosition = worldPositionRoughness.xyz;
  data.ViewSpacePosition = viewSpacePositionDepth.xyz;
  data.ViewDirection = viewDirection.xyz;
  data.Depth = viewSpacePositionDepth.w;
  data.Metallic = normalMetallic.w;
  data.Roughness = worldPositionRoughness.w;
}
