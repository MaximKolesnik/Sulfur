/******************************************************************************/
/*!
\par     Sulfur
\file    sfTonemap.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/1/2016

\brief   Node for doing final tonemapping of a frame

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTonemap.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Modules/Time/sfTime.hpp"

#include "Modules/Graphics/State/sfBlendState.hpp"
#include "Modules/Graphics/State/sfDepthState.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"

#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Components/sfPointLight.hpp"
#include "Components/sfSpotLight.hpp"
#include "Components/sfDirectionalLight.hpp"
#include "Components/sfMeshRenderer.hpp"

namespace Sulfur
{

Tonemap::Tonemap(D3D11Device& device, RenderTarget *renderTarget, Texture2D *frame)
  : RenderNode(device), m_renderTarget(renderTarget), m_frame(frame)
{
  CreateResources(device);
}

Tonemap::~Tonemap()
{
}

void Tonemap::Process()
{
  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  BlendState::Set(m_context, BlendState::NO_BLENDING);

  RenderLuminance();
  Bloom();
  Composite();

  m_currentLuminanceTarget = !m_currentLuminanceTarget;
}

void Tonemap::CreateResources(D3D11Device& device)
{
  m_toneMapData.Threshold = 0.0f;
  m_toneMapData.WhiteLevel = 5.0f;
  m_toneMapData.Saturation = 1.0f;
  m_toneMapData.Bias = 5.0f;
  m_toneMapData.Exposure = 0.0f;
  m_toneMapData.KeyValue = 0.18f;
  m_toneMapData.AutoExposure = 1.0f;
  m_toneMapData.LuminanceMipLevel = 10.0f;

  m_bloomShader.Init(device, "Shaders/PSBloom.sbin");
  m_bloomDataBuffer = m_bloomShader.GetConstantBuffer("TonemapData");
  m_luminanceShader.Init(device, "Shaders/PSLuminance.sbin");
  m_compositeShader.Init(device, "Shaders/PSComposite.sbin");
  m_compositeDataBuffer = m_compositeShader.GetConstantBuffer("TonemapData");
  m_adaptLuminanceShader.Init(device, "Shaders/PSAdaptLuminance.sbin");
  m_adaptLuminanceDataBuffer = m_adaptLuminanceShader.GetConstantBuffer("AdaptLuminanceData");

  D3D11_TEXTURE2D_DESC description;
  description.Width = 1024;
  description.Height = 1024;
  description.MipLevels = 0;
  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_R32_FLOAT;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
  m_adaptedLuminanceTargets[0].Init(device, description);
  m_adaptedLuminanceTargets[1].Init(device, description);
  m_initialLuminanceTarget.Init(device, description);
  m_currentLuminanceTarget = 0;

  description.MipLevels = 1;
  description.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
  description.MiscFlags = 0;
  m_bloomTarget.Init(device, description);

  m_renderTarget->RegisterCallbackOnResize(this, &Tonemap::OnTargetResized);
}

void Tonemap::RenderLuminance()
{
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);
  m_luminanceShader.Set(m_context);
  m_initialLuminanceTarget.Set(m_context);
  m_frame->SetPixel(m_context, 0);
  GraphicsUtils::RenderFullscreenQuad(m_context);

  AdaptLuminanceData adaptLuminanceData;
  adaptLuminanceData.DeltaTime = Time::Instance()->GetDt();
  adaptLuminanceData.Rate = 1.2f;
  m_adaptLuminanceDataBuffer->SetData(m_context, adaptLuminanceData);

  SamplerState::SetPixel(m_context, SamplerState::POINT, 0);
  m_adaptLuminanceShader.Set(m_context);
  m_adaptedLuminanceTargets[m_currentLuminanceTarget].Set(m_context);
  m_adaptedLuminanceTargets[!m_currentLuminanceTarget].GetTexture()->SetPixel(m_context, 0);
  m_initialLuminanceTarget.GetTexture()->SetPixel(m_context, 1);
  GraphicsUtils::RenderFullscreenQuad(m_context);

  m_context.ResetRenderTargets();
  m_adaptedLuminanceTargets[m_currentLuminanceTarget].GetTexture()->GenerateMips(m_context);
}

void Tonemap::Bloom()
{
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  m_toneMapData.Threshold = 2.0f;
  m_bloomDataBuffer->SetData(m_context, m_toneMapData);

  m_bloomShader.Set(m_context);
  m_bloomTarget.Set(m_context);
  m_frame->SetPixel(m_context, 0);
  m_adaptedLuminanceTargets[m_currentLuminanceTarget].GetTexture()->SetPixel(m_context, 1);
  GraphicsUtils::RenderFullscreenQuad(m_context);

  m_bloomMap = m_bloomTarget.GetTexture()->Blurred(m_device, m_context, DXGI_FORMAT_R16G16B16A16_FLOAT, false, BLOOM_DOWNSCALE_STEPS, BLOOM_BLUR_ITERATIONS);
}

void Tonemap::Composite()
{
  SamplerState::SetPixel(m_context, SamplerState::POINT, 0);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 1);

  m_toneMapData.Threshold = 0.0f;
  if (m_compositeDataBuffer) m_compositeDataBuffer->SetData(m_context, m_toneMapData);

  m_compositeShader.Set(m_context);
  m_renderTarget->Set(m_context);
  m_frame->SetPixel(m_context, 0);
  m_adaptedLuminanceTargets[m_currentLuminanceTarget].GetTexture()->SetPixel(m_context, 1);
  m_bloomMap->SetPixel(m_context, 2);
  GraphicsUtils::RenderFullscreenQuad(m_context);
}

void Tonemap::OnTargetResized(UINT32 width, UINT32 height)
{
  m_bloomTarget.Resize(width, height);
}

}