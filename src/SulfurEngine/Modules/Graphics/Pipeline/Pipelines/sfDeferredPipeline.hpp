/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredPipeline.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Deferred pipeline

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderPipeline.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"

namespace Sulfur
{
	
  class DeferredPipeline : public RenderPipeline
  {
  
  public:
    DeferredPipeline(D3D11Device& device, RenderTarget *renderTarget);
    ~DeferredPipeline();

  private:
    RenderTarget *m_renderTarget;

  };
  
}