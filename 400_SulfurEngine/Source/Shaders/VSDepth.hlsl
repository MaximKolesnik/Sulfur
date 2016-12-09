/******************************************************************************/
/*!
\par     Sulfur
\file    VSDepth.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Vertex shader for rendering depth.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

DepthPixelIn main(float4 position : POSITION)
{
  DepthPixelIn output;

  output.position = mul(position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.depth = dot(output.position.xyz, output.position.xyz);
  output.position = mul(output.position, ProjMatrix);

  return output;
}