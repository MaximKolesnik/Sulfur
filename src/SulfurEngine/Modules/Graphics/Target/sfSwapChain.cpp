/******************************************************************************/
/*!
\par     Sulfur
\file    sfSwapChain.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Swap chain

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSwapChain.hpp"
#include "Modules/Graphics/Core/sfD3D11Factory.hpp"

namespace Sulfur
{

void SwapChain::Init(D3D11Device& device, DXGI_SWAP_CHAIN_DESC& description)
{
  WrapperBase::Init(D3D11Factory::Instance()->CreateSwapChain(device.GetD3DResource(), description));

  ID3D11Texture2D *buffer = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    m_resource->GetBuffer(0, IID_ID3D11Texture2D, (void**)&buffer),
    "Failed to get back buffer from swap chain."
    );

  m_backBuffer.Init(buffer);
}

void SwapChain::Present(bool vsync)
{
  m_resource->Present(vsync ? 1 : 0, 0);
}

Texture2D& SwapChain::GetBackBuffer()
{
  return m_backBuffer;
}

}
