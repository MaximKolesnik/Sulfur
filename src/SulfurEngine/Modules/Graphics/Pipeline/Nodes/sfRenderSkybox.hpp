/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderSkybox.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   Node for rendering the skybox to the main target

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
#include "Modules/Graphics/Resources/Texture/sfCubeMap.hpp"

namespace Sulfur
{

  class Scene;

  class RenderSkybox : public RenderNode
  {
  
  public:
    RenderSkybox(D3D11Device& device, RenderTarget *renderTarget);
    ~RenderSkybox();

    virtual void Process() override;

  private:
    void SetupCamera(Scene& scene);

  private:
    D3D11VertexShader m_vertexShader;
    D3D11ConstantBuffer *m_perFrameData;
    D3D11ConstantBuffer *m_perObjectData;

    D3D11PixelShader m_pixelShader;

    RenderTarget *m_renderTarget;
    Mesh m_skyboxMesh;
    CubeMap *m_skyboxMap;

  };
  
}