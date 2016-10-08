cbuffer PerFrameData
{
  matrix ViewMatrix;
  matrix ProjMatrix;
  float3 ViewPosition;
};

cbuffer PerObjectData
{
  matrix WorldMatrix;
};

struct VertexIn
{
  float4 position : POSITION;
  float4 texCoords : TEXCOORD0;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float3 binormal : BINORMAL;
  float4 color : COLOR;
};