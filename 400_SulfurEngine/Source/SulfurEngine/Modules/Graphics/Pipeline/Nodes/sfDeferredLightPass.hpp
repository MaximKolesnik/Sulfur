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
#include "Modules/Graphics/Target/sfGBuffer.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Modules/Graphics/Resources/Texture/sfCubeMap.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Scene/sfSceneManager.hpp"

namespace Sulfur
{

class DeferredLightPass : public RenderNode
{

public:
  DeferredLightPass(D3D11Device& device, RenderTarget *renderTarget, GBuffer *gbuffer);
  ~DeferredLightPass();

  virtual void Process() override;

private:
  void PrepShadowMap(RenderTarget& shadowMap, UINT32 index = 0);
  void PrepLightPass(D3D11PixelShader *pixelShader, Texture2D *shadowMap, int shadowMapSlot);

  void RenderDirectionalLights(Matrix4& view, Matrix4& camera);
  void RenderPointLights(Matrix4& view, Matrix4& camera);
  void RenderSpotLights(Matrix4& view, Matrix4& camera);

private:
  D3D11VertexShader m_fullscreenQuadVertexShader;

  D3D11PixelShader m_directionalLightShader;
  D3D11ConstantBuffer *m_directionalLightData;
  int m_dlShadowMapRegister;

  D3D11PixelShader m_pointLightPixelShader;
  D3D11ConstantBuffer *m_pointLightData;
  int m_plShadowMapRegister;

  D3D11PixelShader m_spotLightPixelShader;
  D3D11ConstantBuffer *m_spotLightData;
  int m_slShadowMapRegister;

  D3D11VertexShader m_depthVertexShader;
  D3D11ConstantBuffer *m_perFrameData;
  D3D11ConstantBuffer *m_perObjectData;

  D3D11PixelShader m_depthPixelShader;

  RenderTarget *m_renderTarget;
  GBuffer *m_gbuffer;

  RenderTarget m_shadowMapTarget;
  Texture2D m_shadowMap;

  RenderTarget m_shadowCubeTarget;
  CubeMap m_shadowCube;

  DepthBuffer m_shadowMapDepthBuffer;

};

}