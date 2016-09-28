/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderGbuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the scene to a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Components/sfMeshRenderer.hpp"

namespace Sulfur
{
	
  class RenderGbuffer : public RenderNode
  {
  
  public:
    RenderGbuffer(D3D11Device& device, RenderTarget *renderTarget);
    ~RenderGbuffer();

    virtual void Process() override;

  private:
    void RenderMeshRenderer(MeshRenderer *meshRenderer);

  private:
    D3D11VertexShader m_vertexShader;
    D3D11ConstantBuffer *m_perFrameData;
    D3D11ConstantBuffer *m_perObjectData;

    D3D11PixelShader m_pixelShader;

    RenderTarget *m_renderTarget;
    Mesh *m_boxMesh;

    Texture2D *m_texture;

  };
  
}