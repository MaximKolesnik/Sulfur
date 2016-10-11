/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredLightPass.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the light pass using a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDeferredLightPass.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Scene/sfSceneManager.hpp"

#include "Modules/Graphics/State/sfBlendState.hpp"
#include "Modules/Graphics/State/sfDepthState.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"

#include "Factories/sfComponentFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Components/sfPointLight.hpp"
#include "Components/sfSpotLight.hpp"
#include "Components/sfDirectionalLight.hpp"

namespace Sulfur
{

DeferredLightPass::DeferredLightPass(D3D11Device& device, RenderTarget *renderTarget, Texture2D *gBuffer)
  : RenderNode(device), m_renderTarget(renderTarget), m_gBuffer(gBuffer)
{
  m_fullscreenQuadVertexShader.Init(device, "Shaders/VSFullScreenQuad.sbin");
  m_ambientPixelShader.Init(device, "Shaders/PSDeferredAmbient.sbin");

  m_directionalLightShader.Init(device, "Shaders/PSDeferredDirectionalLight.sbin");
  m_directionalLightData = m_directionalLightShader.GetConstantBuffer("DirectionalLightData");

  m_pointLightPixelShader.Init(device, "Shaders/PSDeferredPointLight.sbin");
  m_pointLightData = m_pointLightPixelShader.GetConstantBuffer("PointLightData");

  m_spotLightPixelShader.Init(device, "Shaders/PSDeferredSpotLight.sbin");
  m_spotLightData = m_spotLightPixelShader.GetConstantBuffer("SpotLightData");
}

DeferredLightPass::~DeferredLightPass()
{

}

void DeferredLightPass::Process()
{
  m_renderTarget->Set(m_context);
  m_gBuffer->SetPixel(m_context, 0);

  m_fullscreenQuadVertexShader.Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  RasterState::Set(m_context, RasterState::NO_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);
  m_context.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  RenderAmbientLight();
  RenderDirectionalLights();
  RenderPointLights();
  RenderSpotLights();
}

void DeferredLightPass::RenderAmbientLight()
{
  SceneProperties& sceneProps = SceneManager::Instance()->GetScene().m_sceneProperties;
  CubeMap *skyboxMap = sceneProps.GetSkybox();
  if (skyboxMap == nullptr || !sceneProps.GetIbl()) return;

  skyboxMap->SetPixel(m_context, 1);
  skyboxMap->Convolved(m_device)->SetPixel(m_context, 2);

  BlendState::Set(m_context, BlendState::ALPHA);
  m_ambientPixelShader.Set(m_context);

  m_context.Draw(6, 0);
}

void DeferredLightPass::RenderDirectionalLights()
{
  BlendState::Set(m_context, BlendState::ADDITIVE);
  m_directionalLightShader.Set(m_context);

  DirectionalLightData directionalLightData;
  ComponentFactory::ComponentData directionalLights = ComponentFactory::Instance()->GetComponentData<DirectionalLight>();
  for (auto it = directionalLights.begin(); it != directionalLights.end(); ++it)
  {
    DirectionalLight *directionalLight = static_cast<DirectionalLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(directionalLight->GetOwner());

    directionalLightData.Direction = transform->GetWorldForward();
    directionalLightData.Color = directionalLight->GetColor();
    directionalLightData.Intensity = directionalLight->GetIntensity();
    m_directionalLightData->SetData(m_context, directionalLightData);

    m_context.Draw(6, 0);
  }
}

void DeferredLightPass::RenderPointLights()
{
  BlendState::Set(m_context, BlendState::ADDITIVE);
  m_pointLightPixelShader.Set(m_context);

  PointLightData pointLightData;
  ComponentFactory::ComponentData pointLights = ComponentFactory::Instance()->GetComponentData<PointLight>();
  for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
  {
    PointLight *pointLight = static_cast<PointLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(pointLight->GetOwner());
    Vector3 pos = transform->GetWorldTranslation();

    pointLightData.Position = Vector4(pos[0], pos[1], pos[2], 1.0f);
    pointLightData.Color = pointLight->GetColor();
    pointLightData.Range = pointLight->GetRange();
    pointLightData.Intensity = pointLight->GetIntensity();
    m_pointLightData->SetData(m_context, pointLightData);

    m_context.Draw(6, 0);
  }
}

void DeferredLightPass::RenderSpotLights()
{
  BlendState::Set(m_context, BlendState::ADDITIVE);
  m_spotLightPixelShader.Set(m_context);

  SpotLightData spotLightData;
  ComponentFactory::ComponentData spotLights = ComponentFactory::Instance()->GetComponentData<SpotLight>();
  for (auto it = spotLights.begin(); it != spotLights.end(); ++it)
  {
    SpotLight *spotLight = static_cast<SpotLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(spotLight->GetOwner());
    Vector3 pos = transform->GetWorldTranslation();

    spotLightData.Position = Vector4(pos[0], pos[1], pos[2], 1.0f);
    spotLightData.Direction = transform->GetWorldForward();
    spotLightData.Color = spotLight->GetColor();
    spotLightData.Range = spotLight->GetRange();
    spotLightData.Intensity = spotLight->GetIntensity();
    spotLightData.InnerAngle = spotLight->GetInnerAngle() * SF_RADS_PER_DEG;
    spotLightData.OuterAngle = spotLight->GetOuterAngle() * SF_RADS_PER_DEG;
    spotLightData.Falloff = spotLight->GetFalloff();
    m_spotLightData->SetData(m_context, spotLightData);

    m_context.Draw(6, 0);
  }
}

}
