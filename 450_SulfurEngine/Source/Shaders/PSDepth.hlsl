/******************************************************************************/
/*!
\par     Sulfur
\file    PSBlur.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering depth.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

float main(DepthPixelIn input) : SV_TARGET
{
  return input.depth;
}
