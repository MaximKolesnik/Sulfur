/******************************************************************************/
/*!
\par     Sulfur
\file    PSPicking.hlsl
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu

\brief   Pixel shader for rendering picking texture.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "PixelInputs.hlsli"

cbuffer PickingData
{
  uint ObjectHandle;
};

uint main(PixelIn input) : SV_TARGET
{
  return ObjectHandle;
}
