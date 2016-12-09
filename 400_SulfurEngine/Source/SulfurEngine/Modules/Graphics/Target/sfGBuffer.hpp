/******************************************************************************/
/*!
\par     Sulfur
\file    sfGBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/28/2016

\brief   GBuffer render target

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11Shader.hpp"

namespace Sulfur
{
	
enum GBufferTargets
{
  GBUFFER_DIFFUSE_METALLIC,
  GBUFFER_NORMAL_ROUGHNESS,
  GBUFFER_VIEW_POSITION,
  GBufferTargetCount
};

class GBuffer
{

public:
  struct GBufferFrameInfo
  {
    std::string Name;
    DXGI_FORMAT Format;
  };

public:
  void Init(D3D11Device& device, UINT32 width, UINT32 height);
  void Free();

  void Resize(UINT32 width, UINT32 height);

  void SetTarget(D3D11Context& context);
  void SetTarget(D3D11Context& context, DepthBuffer& depthBuffer);
  void SetTexturePixel(D3D11Context& context, D3D11Shader& shader);
  void Clear(D3D11Context& context);

  UINT32 GetWidth() const;
  UINT32 GetHeight() const;

protected:
  static GBufferFrameInfo s_gbufferFrameInfo[GBufferTargetCount];

  D3D11Device *m_device;
  RenderTarget m_renderTargets[GBufferTargetCount];

  SF_CALLBACK(OnResize, UINT32, UINT32)

};
  
}