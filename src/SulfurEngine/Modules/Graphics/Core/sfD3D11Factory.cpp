/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Factory.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 factory

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Factory.hpp"

namespace Sulfur
{

D3D11Factory::D3D11Factory()
{
  IDXGIFactory *factory = nullptr;
  CreateDXGIFactory(IID_IDXGIFactory, (void**)&factory);
  Init(factory);

  // Enum adapters
  IDXGIAdapter *dxgiAdapter = nullptr;
  UINT32 i = 0;
  while (m_resource->EnumAdapters(i++, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
  {
    m_adapters.push_back(D3D11Adapter());
    m_adapters.back().Init(dxgiAdapter);
  }
}

D3D11Factory::~D3D11Factory()
{
  Free();
}

IDXGISwapChain* D3D11Factory::CreateSwapChain(ID3D11Device *device, DXGI_SWAP_CHAIN_DESC& description)
{
  IDXGISwapChain *swapChain = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    m_resource->CreateSwapChain(device, &description, &swapChain),
    "Failed to create swap chain."
    );

  return swapChain;
}

D3D11Factory::AdapterList& D3D11Factory::GetAdapters()
{
  return m_adapters;
}

}
