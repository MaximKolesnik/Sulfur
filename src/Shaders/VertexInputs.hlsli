/******************************************************************************/
/*!
\par     Sulfur
\file    VSFullScreenQuad.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Vertex shader structures and buffers.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
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
  float4 normal : NORMAL;
  float4 tangent : TANGENT;
  float4 binormal : BINORMAL;
  float4 color : COLOR;
};