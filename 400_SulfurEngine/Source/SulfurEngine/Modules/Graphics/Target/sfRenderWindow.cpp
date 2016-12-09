/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderWindow.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Render window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderWindow.hpp"
#include "Modules/Graphics/Core/sfD3D11Factory.hpp"

namespace Sulfur
{

void RenderWindow::Init(D3D11Device& device, Window& window)
{
  m_window = &window;
  m_window->RegisterCallbackOnSize(this, &RenderWindow::Resize);

  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  swapChainDesc.BufferDesc.Width = 0;
  swapChainDesc.BufferDesc.Height = 0;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.SampleDesc.Quality = 0;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;
  swapChainDesc.OutputWindow = window.GetHandle();
  swapChainDesc.Windowed = true;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

  m_swapChain.Init(device, swapChainDesc);
  RenderTarget::Init(device, m_swapChain.GetBackBuffer());
}

void RenderWindow::Free()
{
  m_swapChain.Free();
  RenderTarget::Free();
}

void RenderWindow::Present(bool vsync)
{
  m_swapChain.Present(vsync);
}

void RenderWindow::Resize(UINT32 width, UINT32 height)
{
  WrapperBase::Free();
  m_swapChain.Resize(width, height);
  RenderTarget::Init(*m_device, m_swapChain.GetBackBuffer());

  NotifyOnResize(width, height);
}

}
