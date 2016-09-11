/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredPipeline.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Deferred pipeline

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDeferredPipeline.hpp"

// Nodes
#include "../Nodes/sfRenderGbuffer.hpp"
#include "../Nodes/sfDeferredLightPass.hpp"
#include "../Nodes/sfRenderDebugDraw.hpp"

namespace Sulfur
{

DeferredPipeline::DeferredPipeline(D3D11Device& device, RenderTarget *renderTarget)
{
  m_renderTarget = renderTarget;

  AddNode(new RenderGbuffer(device, renderTarget));
  AddNode(new DeferredLightPass(device));
  AddNode(new RenderDebugDraw(device, renderTarget));
}

DeferredPipeline::~DeferredPipeline()
{

}

}
