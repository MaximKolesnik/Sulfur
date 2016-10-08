/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredLightPass.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the light pass using a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Modules/Graphics/Resources/Texture/sfCubeMap.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"

namespace Sulfur
{

class DeferredLightPass : public RenderNode
{

public:
  DeferredLightPass(D3D11Device& device, RenderTarget *renderTarget, Texture2D *gBuffer);
  ~DeferredLightPass();

  virtual void Process() override;

private:
  void RenderAmbientLight();
  void RenderDirectionalLights();
  void RenderPointLights();
  void RenderSpotLights();

private:
  D3D11VertexShader m_fullscreenQuadVertexShader;
  D3D11PixelShader m_ambientPixelShader;

  D3D11PixelShader m_directionalLightShader;
  D3D11ConstantBuffer *m_directionalLightData;

  D3D11PixelShader m_pointLightPixelShader;
  D3D11ConstantBuffer *m_pointLightData;

  D3D11PixelShader m_spotLightPixelShader;
  D3D11ConstantBuffer *m_spotLightData;

  RenderTarget *m_renderTarget;
  Texture2D *m_gBuffer;
  CubeMap *m_skyboxMap;
  CubeMap m_convolvedSkyboxMap;

};

}