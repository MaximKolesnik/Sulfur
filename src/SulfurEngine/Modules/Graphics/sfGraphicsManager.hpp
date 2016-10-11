/******************************************************************************/
/*!
\par     Sulfur
\file    sfGraphicsManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Graphics Manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "Modules/Window/sfWindow.hpp"
#include "Core/sfD3D11Device.hpp"
#include "Target/sfRenderWindow.hpp"
#include "Pipeline/sfRenderPipeline.hpp"

namespace Sulfur
{
	
  class GraphicsManager
  {
    SF_SINGLETON(GraphicsManager)
  public:
    void Init(Window& window);
    void Free();

    void Update();

    D3D11Device& GetDevice();

  private:
    void InitDevice();

  private:

    Window *m_window;
    D3D11Device m_device;

    RenderWindow m_renderWindow;
    RenderPipeline *m_pipeline;
  
  };
  
}