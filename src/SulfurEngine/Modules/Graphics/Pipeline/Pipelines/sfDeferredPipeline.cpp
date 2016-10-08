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
#include "../Data/sfDeferredData.hpp"

// Nodes
#include "../Nodes/sfRenderGbuffer.hpp"
#include "../Nodes/sfDeferredLightPass.hpp"
#include "../Nodes/sfRenderDebugDraw.hpp"
#include "../Nodes/sfRenderSkybox.hpp"

namespace Sulfur
{

DeferredPipeline::DeferredPipeline(D3D11Device& device, RenderTarget *renderTarget)
{
  m_finalRenderTarget = renderTarget;
  CreateGBuffer(device);
  CreateDepthBuffer(device);

  AddNode(new RenderSkybox(device, m_finalRenderTarget));
  AddNode(new RenderGbuffer(device, &m_gBuffer, &m_depthBuffer));
  AddNode(new DeferredLightPass(device, m_finalRenderTarget, m_gBuffer.GetTexture()));
  AddNode(new RenderDebugDraw(device, m_finalRenderTarget, &m_depthBuffer));
}

DeferredPipeline::~DeferredPipeline()
{
  m_depthBuffer.Free();
  m_depthBuffer.Free();
}

void DeferredPipeline::CreateGBuffer(D3D11Device& device)
{
  D3D11_TEXTURE2D_DESC description = m_finalRenderTarget->GetTexture()->GetDescription();
  description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  description.ArraySize = GBufferTargetCount;
  description.MipLevels = 1;
  description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  m_gBuffer.Init(device, description);

  m_finalRenderTarget->RegisterCallbackOnResize(&m_gBuffer, &RenderTarget::Resize);
}

void DeferredPipeline::CreateDepthBuffer(D3D11Device& device)
{
  D3D11_TEXTURE2D_DESC description = m_finalRenderTarget->GetTexture()->GetDescription();
  description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  description.ArraySize = 1;
  description.MipLevels = 1;
  description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  m_depthBuffer.Init(device, description);

  m_finalRenderTarget->RegisterCallbackOnResize(&m_depthBuffer, &DepthBuffer::Resize);
}

}
