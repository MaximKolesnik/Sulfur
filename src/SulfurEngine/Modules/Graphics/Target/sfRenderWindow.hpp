/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderWindow.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Render window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfRenderTarget.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Window/sfWindow.hpp"
#include "sfSwapChain.hpp"

namespace Sulfur
{
	
  class RenderWindow : public RenderTarget
  {
  
  public:
    void Init(D3D11Device& device, Window& window);
    virtual void Free() override;

    void Present(bool vsync = false);

  private:
    Window *m_window;
    SwapChain m_swapChain;

  };
  
}