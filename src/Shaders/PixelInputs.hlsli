cbuffer MaterialData
{
  float4 MaterialDiffuseColor;
  float4 MaterialEmissiveColor;
  bool MaterialUsesDiffuseTexture;
  bool MaterialUsesNormalTexture;
  bool MaterialUsesMaterialTexture;
  bool MaterialUsesEmissiveTexture;
  float MaterialMetallic;
  float MaterialRoughness;
};

struct PixelIn
{
  float4 position : SV_POSITION;
  float2 texCoords : TEXCOORD;
  float4 color : COLOR;
};

struct DeferredPixelIn
{
  float4 position : SV_POSITION;
  float2 texCoords : TEXCOORD;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float3 binormal : BINORMAL;
  float3 worldPosition : TEXCOORD1;
  float3 viewSpacePosition : TEXCOORD2;
  float3 viewDirection : TEXCOORD3;
  float4 color : COLOR;
};

struct SkyboxPixelIn
{
  float4 position : SV_POSITION;
  float3 texCoords : TEXCOORD;
};

struct DepthPixelIn
{
  float4 position : SV_POSITION;
  float depth : TEXCOORD0;
};
