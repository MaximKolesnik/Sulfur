/******************************************************************************/
/*!
\par     Sulfur
\file    sfTonemap.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/1/2016

\brief   Node for doing final tonemapping of a frame

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"

namespace Sulfur
{

class Tonemap : public RenderNode
{

public:
  Tonemap(D3D11Device& device, RenderTarget *renderTarget, Texture2D *frame);
  ~Tonemap();

  virtual void Process() override;

private:
  void CreateResources(D3D11Device& device);
  void RenderLuminance();
  void Bloom();
  void Composite();
  void OnTargetResized(UINT32 width, UINT32 height);

private:
  TonemapData m_toneMapData;

  RenderTarget *m_renderTarget;
  Texture2D *m_frame;

  static const UINT32 LUMINANCE_TEXTURE_SIZE_DIVISOR = 2;
  D3D11PixelShader m_luminanceShader;
  D3D11PixelShader m_adaptLuminanceShader;
  D3D11ConstantBuffer *m_adaptLuminanceDataBuffer;
  RenderTarget m_initialLuminanceTarget;
  RenderTarget m_adaptedLuminanceTargets[2];
  UINT32 m_currentLuminanceTarget;

  static const UINT32 BLOOM_BLUR_ITERATIONS = 4;
  static const UINT32 BLOOM_DOWNSCALE_STEPS = 2;
  D3D11PixelShader m_bloomShader;
  D3D11ConstantBuffer *m_bloomDataBuffer;
  RenderTarget m_bloomTarget;
  Texture2D *m_bloomMap;

  D3D11PixelShader m_compositeShader;
  D3D11ConstantBuffer *m_compositeDataBuffer;

};

}