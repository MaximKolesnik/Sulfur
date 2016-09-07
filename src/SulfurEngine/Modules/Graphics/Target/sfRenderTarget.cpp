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
  
  // TODO: Add multiple render targets for texture arrays
  D3D11_RENDER_TARGET_VIEW_DESC rtvDescription;
  rtvDescription.Format = texture.GetDescription().Format;
  rtvDescription.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  rtvDescription.Texture2D.MipSlice = 0;

  ID3D11RenderTargetView *rtv = nullptr;
  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateRenderTargetView(texture.GetD3DResource(), &rtvDescription, &rtv),
    "Failed to create render target view."
    );

  WrapperBase::Init(rtv);
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
}

void RenderTarget::Set(D3D11Context& context)
{
  context.GetD3DResource()->OMSetRenderTargets(1, &m_resource, nullptr);
}

void RenderTarget::Set(D3D11Context& context, DepthBuffer& depthBuffer)
{
  context.GetD3DResource()->OMSetRenderTargets(1, &m_resource, depthBuffer.GetD3DResource());
}

void RenderTarget::Clear(D3D11Context& context, const Vector4& color)
{
  FLOAT c[4] = { color.GetX(), color.GetY(), color.GetZ(), color.GetW() };
  context.GetD3DResource()->ClearRenderTargetView(m_resource, c);
}

}
