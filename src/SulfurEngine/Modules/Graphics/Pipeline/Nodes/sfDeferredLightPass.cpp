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

  m_depthShader.Init(device, "Shaders/VSDepth.sbin");
  m_perFrameData = m_depthShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_depthShader.GetConstantBuffer("PerObjectData");

  D3D11_TEXTURE2D_DESC description;
  description.Width = EngineSettings::ShadowMapSize;
  description.Height = EngineSettings::ShadowMapSize;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_R24G8_TYPELESS;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;
  m_shadowMap.Init(device, description);

  description.ArraySize = 6;
  description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  m_shadowCube.Init(device, description);
}

DeferredLightPass::~DeferredLightPass()
{
  m_shadowMap.Free();
  m_shadowCube.Free();
}

void DeferredLightPass::Process()
{
  RenderAmbientLight();
  RenderDirectionalLights();
  RenderPointLights();
  RenderSpotLights();
}

void DeferredLightPass::RenderWorld()
{
  ComponentFactory::ComponentData componentData = SF_GET_COMP_DATA(MeshRenderer);
  for (auto it = componentData.begin(); it != componentData.end(); ++it)
  {
    Object *object = SF_GET_OBJECT(it->GetOwner());
    Transform* transform = object->GetComponent<Transform>();

    PerObjectData perObject;
    perObject.WorldMatrix = transform->GetWorldMatrix();
    m_perObjectData->SetData(m_context, perObject);

    static_cast<MeshRenderer*>(*it)->GetMesh()->Draw(m_context);
  }
}

void DeferredLightPass::RenderAmbientLight()
{
  SceneProperties& sceneProps = SceneManager::Instance()->GetScene().m_sceneProperties;
  CubeMap *skyboxMap = sceneProps.GetSkybox();
  if (skyboxMap == nullptr || !sceneProps.GetIbl()) return;

  PrepLightPass(&m_ambientPixelShader);
  skyboxMap->SetPixel(m_context, 1);
  skyboxMap->Convolved(m_device)->SetPixel(m_context, 2);
  BlendState::Set(m_context, BlendState::ALPHA);

  m_context.Draw(6, 0);
}

void DeferredLightPass::RenderDirectionalLights()
{
  PrepLightPass(&m_directionalLightShader);

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
  PrepLightPass(&m_pointLightPixelShader);

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
  PrepLightPass(&m_spotLightPixelShader);

  SpotLightData spotLightData;
  PerFrameData perFrameData;
  ComponentFactory::ComponentData spotLights = ComponentFactory::Instance()->GetComponentData<SpotLight>();
  for (auto it = spotLights.begin(); it != spotLights.end(); ++it)
  {
    SpotLight *spotLight = static_cast<SpotLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(spotLight->GetOwner());
    Vector3 pos = transform->GetWorldTranslation();

    PrepShadowMap();
    perFrameData.ProjMatrix.SetPerspectiveFovLH((float)EngineSettings::ShadowMapSize, (float)EngineSettings::ShadowMapSize, spotLight->GetOuterAngle(), 0.01f, spotLight->GetRange());
    perFrameData.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), pos);
    m_perFrameData->SetData(m_context, perFrameData);
    RenderWorld();

    PrepLightPass(&m_spotLightPixelShader);
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

void DeferredLightPass::PrepShadowMap()
{
  m_shadowMap.Clear(m_context);
  m_shadowMap.Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_ENABLED);
  RasterState::Set(m_context, RasterState::NO_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);
}

void DeferredLightPass::PrepLightPass(D3D11PixelShader *pixelShader)
{
  m_renderTarget->Set(m_context);
  m_gBuffer->SetPixel(m_context, 0);

  m_fullscreenQuadVertexShader.Set(m_context);
  pixelShader->Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  RasterState::Set(m_context, RasterState::NO_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);
  BlendState::Set(m_context, BlendState::ADDITIVE);
  m_context.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

}
