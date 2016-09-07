cbuffer PerFrameData
{
  matrix ViewMatrix;
  matrix ProjMatrix;
};

cbuffer PerObjectData
{
  matrix WorldMatrix;
};

struct VertexIn
{
  float4 position : POSITION;
  float4 texCoords : TEXCOORD0;
  float4 color : COLOR;
};