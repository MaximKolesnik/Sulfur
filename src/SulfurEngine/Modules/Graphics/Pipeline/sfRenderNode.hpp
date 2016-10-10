/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderNode.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Base render pipeline node

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class RenderNode
  {
  
  public:
    RenderNode(D3D11Device& device);
    virtual ~RenderNode();

    virtual void Process() = 0;

    D3D11Context& GetContext();

  protected:
    D3D11Device& m_device;
    D3D11Context m_context;

  };
  
}