/******************************************************************************/
/*!
\par     Sulfur
\file    PSWireframe.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Test pixel shader for wireframe.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

float4 main(PixelIn input) : SV_TARGET
{
  return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
