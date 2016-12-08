/******************************************************************************/
/*!
\par     Sulfur
\file    VSTest.hlsli
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Test vertex shader.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

PixelIn main(VertexIn input)
{
  PixelIn output;

  output.position = mul(input.position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.position = mul(output.position, ProjMatrix);

  output.texCoords = input.texCoords.xy;
  output.color = input.color;

  return output;
}