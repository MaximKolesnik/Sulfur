Texture2D GB_DiffuseMetallic;
Texture2D GB_NormalRoughness;
Texture2D GB_ViewPosition;

struct GBufferOutput
{
  float4 DiffuseMetallic : SV_Target0;
  float4 NormalRoughness : SV_Target1;
  float4 ViewPosition : SV_Target2;
};

struct GBufferData
{
  float3 Diffuse;
  float3 Normal;
  float Depth;
  float3 ViewPosition;
  float Metallic;
  float Roughness;
};

float3 UnpackGBufferDiffuse(in SamplerState ss, in float2 uv)
{
  return GB_DiffuseMetallic.Sample(ss, uv).rgb;
}

float3 UnpackGBufferNormal(in SamplerState ss, in float2 uv)
{
  return GB_NormalRoughness.Sample(ss, uv).xyz;
}

float3 UnpackGBufferViewPosition(in SamplerState ss, in float2 uv)
{
  return GB_ViewPosition.Sample(ss, uv).xyz;
}

float UnpackGBufferDepth(in SamplerState ss, in float2 uv)
{
  return length(UnpackGBufferViewPosition(ss, uv));
}

void UnpackGBuffer(in SamplerState ss, in float2 uv, out GBufferData data)
{
  float4 diffuseMetallic = GB_DiffuseMetallic.Sample(ss, uv);
  float4 normalRoughness = GB_NormalRoughness.Sample(ss, uv);

  data.Diffuse = diffuseMetallic.rgb;
  data.Metallic = diffuseMetallic.a;
  data.Normal = normalRoughness.rgb;
  data.Roughness = normalRoughness.a;
  data.ViewPosition = UnpackGBufferViewPosition(ss, uv);
  data.Depth = length(data.ViewPosition);
}
