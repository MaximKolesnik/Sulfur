/******************************************************************************/
/*!
\par     Sulfur
\file    sfDepthBuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Depth buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDepthBuffer.hpp"

namespace Sulfur
{

void DepthBuffer::Init(D3D11Device& device, Texture2D& texture)
{
  m_texture = texture;
  m_device = &device;

  // TODO: Add support for texture arrays
  D3D11_DEPTH_STENCIL_VIEW_DESC dsvDescription;
  dsvDescription.Format = texture.GetDescription().Format;
  dsvDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsvDescription.Flags = 0;
  dsvDescription.Texture2D.MipSlice = 0;

  ID3D11DepthStencilView *dsv = nullptr;
  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateDepthStencilView(texture.GetD3DResource(), &dsvDescription, &dsv),
    "Failed to create render target view."
    );

  WrapperBase::Init(dsv);
}

void DepthBuffer::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description)
{
  Texture2D texture;
  texture.Init(device, description);
  Init(device, texture);
}

void DepthBuffer::Free()
{
  m_texture.Free();
  WrapperBase::Free();
}

void DepthBuffer::Resize(UINT32 width, UINT32 height)
{
  D3D11_TEXTURE2D_DESC description = m_texture.GetDescription();
  description.Width = width;
  description.Height = height;

  Free();
  Init(*m_device, description);
}

void DepthBuffer::Clear(D3D11Context& context)
{
  context.GetD3DResource()->ClearDepthStencilView(m_resource, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

}
