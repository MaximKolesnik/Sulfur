/******************************************************************************/
/*!
\par     Sulfur
\file    VSFullScreenQuad.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Vertex shader for rendering a fullscreen quad.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

cbuffer ScreenData
{
  float4 Frustum[4];
};

static const float2 TexCoords[4] = {
  float2(0, 0), float2(1, 0), float2(0, 1), float2(1, 1)
};

static const float2 Positions[4] = {
  float2(-1.0f,  1.0f), float2( 1.0f,  1.0f), float2(-1.0f, -1.0f), float2( 1.0f, -1.0f)
};

ScreenPixelIn main(uint vertexId : SV_VERTEXID)
{
  ScreenPixelIn output;

  output.texCoords = TexCoords[vertexId];
  output.position = float4(Positions[vertexId], 0, 1);
  output.viewDir = Frustum[vertexId].xyz;

  return output;
}
