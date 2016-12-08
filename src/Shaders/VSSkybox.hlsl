/******************************************************************************/
/*!
\par     Sulfur
\file    VSSkybox.hlsli
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Vertex shader for rendering skybox.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "VertexInputs.hlsli"
#include "PixelInputs.hlsli"

SkyboxPixelIn main(VertexIn input)
{
  SkyboxPixelIn output;

  output.position = mul(input.position, WorldMatrix);
  output.position = mul(output.position, ViewMatrix);
  output.position = mul(output.position, ProjMatrix);
  output.position.z = output.position.w - 0.01f;

  output.texCoords = normalize(input.position.xyz);

  return output;
}