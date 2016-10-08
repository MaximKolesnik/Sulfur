/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderTarget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Render target

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderTarget.hpp"

namespace Sulfur
{

void RenderTarget::Init(D3D11Device& device, Texture2D& texture)
{
  m_texture = texture;
  m_device = &device;

  const D3D11_TEXTURE2D_DESC& textureDescription = texture.GetDescription();
  D3D11_RENDER_TARGET_VIEW_DESC rtvDescription;
  rtvDescription.Format = textureDescription.Format;

  ID3D11RenderTargetView **rtv = new ID3D11RenderTargetView*[textureDescription.ArraySize];
  if (textureDescription.ArraySize == 1)
  {
    rtvDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDescription.Texture2D.MipSlice = 0;

    SF_CRITICAL_ERR_HRESULT(
      device.GetD3DResource()->CreateRenderTargetView(texture.GetD3DResource(), &rtvDescription, rtv),
      "Failed to create render target view."
      );
  }
  else
  {
    rtvDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    rtvDescription.Texture2DArray.MipSlice = 0;
    rtvDescription.Texture2DArray.ArraySize = 1;

    for (UINT32 i = 0; i < textureDescription.ArraySize; ++i)
    {
      rtvDescription.Texture2DArray.FirstArraySlice = i;

      SF_CRITICAL_ERR_HRESULT(
        device.GetD3DResource()->CreateRenderTargetView(texture.GetD3DResource(), &rtvDescription, rtv + i),
        "Failed to create render target view."
        );
    }
  }

  WrapperBase::Init(rtv, textureDescription.ArraySize);
}

void RenderTarget::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description)
{
  Texture2D texture;
  texture.Init(device, description);
  Init(device, texture);
}

void RenderTarget::Free()
{
  m_texture.Free();
  WrapperBase::Free();
}

void RenderTarget::Resize(UINT32 width, UINT32 height)
{
  D3D11_TEXTURE2D_DESC description = m_texture.GetDescription();
  description.Width = width;
  description.Height = height;

  Free();
  Init(*m_device, description);

  NotifyOnResize(width, height);
}

void RenderTarget::Set(D3D11Context& context)
{
  Set(context, 0, m_resourceCount);
}

void RenderTarget::Set(D3D11Context& context, UINT32 start, UINT32 count)
{
  context.GetD3DResource()->OMSetRenderTargets(count, m_resource + start, nullptr);
  context.SetViewport((Real)m_texture.GetDescription().Width, (Real)m_texture.GetDescription().Height);
}

void RenderTarget::Set(D3D11Context& context, DepthBuffer& depthBuffer)
{
  Set(context, depthBuffer, 0, m_resourceCount);
}


void RenderTarget::Set(D3D11Context& context, DepthBuffer& depthBuffer, UINT32 start, UINT32 count)
{
  context.GetD3DResource()->OMSetRenderTargets(count, m_resource + start, depthBuffer.GetD3DResource());
  context.SetViewport((Real)m_texture.GetDescription().Width, (Real)m_texture.GetDescription().Height);
}

void RenderTarget::Clear(D3D11Context& context, const Vector4& color)
{
  for (UINT32 i = 0; i < m_resourceCount; ++i)
    context.GetD3DResource()->ClearRenderTargetView(m_resource[i], &color[0]);
}

Texture2D* RenderTarget::GetTexture()
{
  return &m_texture;
}

const Texture2D* RenderTarget::GetTexture() const
{
  return &m_texture;
}

}
