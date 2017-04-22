/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredAmbient.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/7/2016

\brief   Node for rendering ambient light with occlusion

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"
#include "Modules/Graphics/Target/sfGBuffer.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"

namespace Sulfur
{

class DeferredAmbient : public RenderNode
{

public:
  DeferredAmbient(D3D11Device& device, RenderTarget *renderTarget, GBuffer *gbuffer);
  ~DeferredAmbient();

  virtual void Process() override;

private:
  void RenderOcclusionMap();
  void RenderAmbientLight();
  void OnTargetResized(UINT32 width, UINT32 height);

private:
  static const UINT32 SSAO_MAP_SIZE_DIVISOR = 2;
  static const UINT32 SSAO_BLUR_ITERATIONS = 1;
  static const UINT32 SSAO_DOWNSCALE_STEPS = 1;
  static const DXGI_FORMAT SSAO_FORMAT = DXGI_FORMAT_R8_UNORM;

  RenderTarget m_occlusionMap;
  D3D11PixelShader m_occlusionPixelShader;
  D3D11ConstantBuffer *m_occlusionDataBuffer;
  OcclusionData m_occlusionData;

  D3D11PixelShader m_ambientPixelShader;
  D3D11ConstantBuffer *m_ambientLightData;
  int m_environmentCubeRegister;
  int m_irradianceCubeRegister;
  int m_occlusionMapRegister;

  RenderTarget *m_renderTarget;
  GBuffer *m_gbuffer;


};

}