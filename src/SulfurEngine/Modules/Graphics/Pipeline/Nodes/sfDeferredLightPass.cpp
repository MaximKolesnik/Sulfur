/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredLightPass.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the light pass using a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDeferredLightPass.hpp"

namespace Sulfur
{

DeferredLightPass::DeferredLightPass(D3D11Device& device)
  : RenderNode(device)
{

}

DeferredLightPass::~DeferredLightPass()
{

}

void DeferredLightPass::Process()
{

}

}
