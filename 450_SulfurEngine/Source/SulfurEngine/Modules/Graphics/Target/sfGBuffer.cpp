/******************************************************************************/
/*!
\par     Sulfur
\file    sfGBuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/28/2016

\brief   GBuffer render target

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGBuffer.hpp"

namespace Sulfur
{

GBuffer::GBufferFrameInfo GBuffer::s_gbufferFrameInfo[GBufferTargetCount] = {
  { "GB_DiffuseMetallic", DXGI_FORMAT_R16G16B16A16_FLOAT },
  { "GB_NormalRoughness", DXGI_FORMAT_R32G32B32A32_FLOAT },
  { "GB_ViewPosition", DXGI_FORMAT_R32G32B32A32_FLOAT }
};

void GBuffer::Init(D3D11Device& device, UINT32 width, UINT32 height)
{
  m_device = &device;

  D3D11_TEXTURE2D_DESC description;
  description.Width = width;
  description.Height = height;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;

  for (UINT32 i = 0; i < GBufferTargetCount; ++i)
  {
    description.Format = s_gbufferFrameInfo[i].Format;
    m_renderTargets[i].Init(device, description);
  }
}

void GBuffer::Free()
{
  for (UINT32 i = 0; i < GBufferTargetCount; ++i)
    m_renderTargets[i].Free();
}

void GBuffer::Resize(UINT32 width, UINT32 height)
{
  Free();
  Init(*m_device, width, height);

  NotifyOnResize(width, height);
}

void GBuffer::SetTarget(D3D11Context& context)
{
  ID3D11RenderTargetView *renderTargets[8] = { nullptr };
  for (UINT32 i = 0; i < GBufferTargetCount; ++i)
    renderTargets[i] = m_renderTargets[i].GetD3DResource()[0];

  context.GetD3DResource()->OMSetRenderTargets(8, renderTargets, nullptr);
  context.SetViewport((Real)m_renderTargets[0].GetTexture()->GetDescription().Width, (Real)m_renderTargets[0].GetTexture()->GetDescription().Height);
}

void GBuffer::SetTarget(D3D11Context& context, DepthBuffer& depthBuffer)
{
  ID3D11RenderTargetView *renderTargets[8] = { nullptr };
  for (UINT32 i = 0; i < GBufferTargetCount; ++i)
    renderTargets[i] = m_renderTargets[i].GetD3DResource()[0];

  context.GetD3DResource()->OMSetRenderTargets(8, renderTargets, depthBuffer.GetD3DResource());
  context.SetViewport((Real)GetWidth(), (Real)GetHeight());
}

void GBuffer::SetTexturePixel(D3D11Context& context, D3D11Shader& shader)
{
  for (UINT32 i = 0; i < GBufferTargetCount; ++i)
  {
    int slot = shader.GetTextureRegister(s_gbufferFrameInfo[i].Name);

    if (slot != -1)
      m_renderTargets[i].GetTexture()->SetPixel(context, slot);
  }
}

void GBuffer::Clear(D3D11Context& context)
{
  m_renderTargets[GBUFFER_DIFFUSE_METALLIC].Clear(context, Vector4(0.5f, 0.5f, 0.5f, 0.0f));
  m_renderTargets[GBUFFER_NORMAL_ROUGHNESS].Clear(context, Vector4(0.0, 0.0f, 0.0f, 0.0f));
  m_renderTargets[GBUFFER_VIEW_POSITION].Clear(context, Vector4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX));
}

UINT32 GBuffer::GetWidth() const
{
  return m_renderTargets[0].GetTexture()->GetDescription().Width;
}

UINT32 GBuffer::GetHeight() const
{
  return m_renderTargets[0].GetTexture()->GetDescription().Height;
}

}
