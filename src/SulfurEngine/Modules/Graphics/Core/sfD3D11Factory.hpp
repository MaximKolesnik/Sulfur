/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Factory.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 factory

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "sfD3D11Wrapper.hpp"
#include "sfD3D11Adapter.hpp"

namespace Sulfur
{
	
  class D3D11Factory : public D3D11ResourceWrapper<IDXGIFactory>
  {
    SF_SINGLETON(D3D11Factory)

  public:
    typedef std::vector<D3D11Adapter> AdapterList;

  public:
    IDXGISwapChain* CreateSwapChain(ID3D11Device *device, DXGI_SWAP_CHAIN_DESC& description);

    AdapterList& GetAdapters();

  private:
    AdapterList m_adapters;

  };
  
}