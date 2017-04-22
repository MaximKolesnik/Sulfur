/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredAmbient.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the light pass using a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDeferredAmbient.hpp"
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

DeferredAmbient::DeferredAmbient(D3D11Device& device, RenderTarget *renderTarget, GBuffer *gbuffer)
  : RenderNode(device), m_renderTarget(renderTarget), m_gbuffer(gbuffer)
{
  renderTarget->RegisterCallbackOnResize(this, &DeferredAmbient::OnTargetResized);

  m_occlusionPixelShader.Init(device, "Shaders/PSSSAO.sbin");
  m_occlusionDataBuffer = m_occlusionPixelShader.GetConstantBuffer("OcclusionData");
  m_occlusionData.Radius = 1.0f;
  m_occlusionData.Amount = 0.6f;
  m_occlusionData.DepthThreshold = 0.0f;
  m_occlusionData.Contrast = 1.0f;
  m_occlusionData.MaxDistance = 100.0f;
  m_occlusionData.Phase = 0.0f;

  m_ambientPixelShader.Init(device, "Shaders/PSDeferredAmbient.sbin");
  m_ambientLightData = m_ambientPixelShader.GetConstantBuffer("AmbientLightData");
  m_environmentCubeRegister = m_ambientPixelShader.GetTextureRegister("TEX_EnvironmentCube");
  m_irradianceCubeRegister = m_ambientPixelShader.GetTextureRegister("TEX_IrradianceCube");
  m_occlusionMapRegister = m_ambientPixelShader.GetTextureRegister("TEX_OcclusionMap");

  D3D11_TEXTURE2D_DESC description;
  description.Width = m_renderTarget->GetTexture()->GetDescription().Width / SSAO_MAP_SIZE_DIVISOR;
  description.Height = m_renderTarget->GetTexture()->GetDescription().Height / SSAO_MAP_SIZE_DIVISOR;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.Format = SSAO_FORMAT;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;
  m_occlusionMap.Init(device, description);
}

DeferredAmbient::~DeferredAmbient()
{
  m_occlusionMap.Free();
}

void DeferredAmbient::Process()
{
  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  BlendState::Set(m_context, BlendState::NO_BLENDING);

  RenderOcclusionMap();
  RenderAmbientLight();
}

void DeferredAmbient::RenderOcclusionMap()
{
  m_occlusionMap.Clear(m_context, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
  
  m_occlusionMap.Set(m_context);
  m_gbuffer->SetTexturePixel(m_context, m_occlusionPixelShader);
  m_occlusionPixelShader.Set(m_context);

  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  Scene& scene = SceneManager::Instance()->GetScene();
  if (m_occlusionDataBuffer) m_occlusionDataBuffer->SetData(m_context, m_occlusionData);

  GraphicsUtils::RenderFullscreenQuad(m_context);
}

void DeferredAmbient::RenderAmbientLight()
{
  Scene& scene = SceneManager::Instance()->GetScene();
  SceneProperties& sceneProps = scene.m_sceneProperties;
  CubeMap *skyboxMap = sceneProps.GetSkybox();

  m_occlusionMap.GetTexture()->Blurred(m_device, m_context, SSAO_FORMAT, false, 0, 1)->SetPixel(m_context, m_occlusionMapRegister);
  m_gbuffer->SetTexturePixel(m_context, m_ambientPixelShader);

  m_ambientPixelShader.Set(m_context);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  AmbientLightData ambientLightData;
  GraphicsUtils::GetCameraTransformMatrix(scene, ambientLightData.InverseView);
  ambientLightData.AmbientLight = sceneProps.GetAmbientLight();
  ambientLightData.UseIBL = sceneProps.GetIbl() && skyboxMap;
  m_ambientLightData->SetData(m_context, ambientLightData);

  if (ambientLightData.UseIBL)
  {
    if (m_environmentCubeRegister != -1) skyboxMap->SetPixel(m_context, m_environmentCubeRegister);
    if (m_irradianceCubeRegister != -1) skyboxMap->Blurred(m_device, m_context, DXGI_FORMAT_R8G8B8A8_UNORM, true, 2, 1)->SetPixel(m_context, m_irradianceCubeRegister);
  }
  else
  {
    if (m_environmentCubeRegister != -1) m_context.ResetPixelTexture(m_environmentCubeRegister);
    if (m_irradianceCubeRegister != -1) m_context.ResetPixelTexture(m_irradianceCubeRegister);
  }

  m_renderTarget->Set(m_context);
  GraphicsUtils::RenderFullscreenQuad(m_context);
}

void DeferredAmbient::OnTargetResized(UINT32 width, UINT32 height)
{
  m_occlusionMap.Resize(width / SSAO_MAP_SIZE_DIVISOR, height / SSAO_MAP_SIZE_DIVISOR);
}

}