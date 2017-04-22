/******************************************************************************/
/*!
\par     Sulfur
\file    sfSwapChain.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Swap chain

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"

namespace Sulfur
{
	
  class SwapChain : public D3D11ResourceWrapper<IDXGISwapChain, DXGI_SWAP_CHAIN_DESC>
  {
  
  public:
    void Init(D3D11Device& device, DXGI_SWAP_CHAIN_DESC& description);

    void Present(bool vsync);

    void Resize(UINT32 width, UINT32 height);
    Texture2D& GetBackBuffer();

  private:
    void UpdateBackBuffer();

  private:
    Texture2D m_backBuffer;

  };
  
}