/******************************************************************************/
/*!
\par     Sulfur
\file    PixelInputs.hlsli
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel input structures and buffers.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
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

struct ScreenPixelIn
{
  float4 position : SV_POSITION;
  float2 texCoords : TEXCOORD0;
  float3 viewDir : TEXCOORD1;
};

struct DeferredPixelIn
{
  float4 position : SV_POSITION;
  float2 texCoords : TEXCOORD0;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float3 binormal : BINORMAL;
  float3 viewPosition : TEXCOORD1;
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
