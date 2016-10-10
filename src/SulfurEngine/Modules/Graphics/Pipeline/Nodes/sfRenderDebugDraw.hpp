/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderDebugDraw.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering debug draw objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"

namespace Sulfur
{

  class Scene;
	
  class RenderDebugDraw : public RenderNode
  {
  
  public:
    RenderDebugDraw(D3D11Device& device, RenderTarget *renderTarget, DepthBuffer *depthBuffer);
    ~RenderDebugDraw();

    virtual void Process() override;

  private:
    void SetupCamera(Scene& scene);

  private:
    D3D11VertexShader m_vertexShader;
    D3D11ConstantBuffer *m_perFrameData;
    D3D11ConstantBuffer *m_perObjectData;

    D3D11PixelShader m_pixelShader;
    D3D11PixelShader m_wireframeShader;

    RenderTarget *m_renderTarget;
    DepthBuffer *m_depthBuffer;

  };
  
}