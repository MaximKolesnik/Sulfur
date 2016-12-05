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
#include "Modules/Graphics/Target/sfGBuffer.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Target/sfDepthBuffer.hpp"

namespace Sulfur
{
	
  class DeferredPipeline : public RenderPipeline
  {
  
  public:
    DeferredPipeline(D3D11Device& device, RenderTarget *renderTarget);
    ~DeferredPipeline();

  private:
    void CreateGBuffer(D3D11Device& device);
    void CreateDepthBuffer(D3D11Device& device);

  private:
    RenderTarget *m_finalRenderTarget;
    RenderTarget m_prePostProcessTarget;
    GBuffer m_gBuffer;
    DepthBuffer m_depthBuffer;

  };
  
}