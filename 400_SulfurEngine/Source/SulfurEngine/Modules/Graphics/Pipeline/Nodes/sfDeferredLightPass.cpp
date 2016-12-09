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
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"

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

DeferredLightPass::DeferredLightPass(D3D11Device& device, RenderTarget *renderTarget, GBuffer *gbuffer)
  : RenderNode(device), m_renderTarget(renderTarget), m_gbuffer(gbuffer)
{
  m_fullscreenQuadVertexShader.Init(device, "Shaders/VSFullScreenQuad.sbin");

  m_directionalLightShader.Init(device, "Shaders/PSDeferredDirectionalLight.sbin");
  m_directionalLightData = m_directionalLightShader.GetConstantBuffer("DirectionalLightData");
  m_dlShadowMapRegister = m_directionalLightShader.GetTextureRegister("TEX_ShadowMap");

  m_pointLightPixelShader.Init(device, "Shaders/PSDeferredPointLight.sbin");
  m_pointLightData = m_pointLightPixelShader.GetConstantBuffer("PointLightData");
  m_plShadowMapRegister = m_pointLightPixelShader.GetTextureRegister("TEX_ShadowMap");

  m_spotLightPixelShader.Init(device, "Shaders/PSDeferredSpotLight.sbin");
  m_spotLightData = m_spotLightPixelShader.GetConstantBuffer("SpotLightData");
  m_slShadowMapRegister = m_spotLightPixelShader.GetTextureRegister("TEX_ShadowMap");

  m_depthVertexShader.Init(device, "Shaders/VSDepth.sbin");
  m_perFrameData = m_depthVertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_depthVertexShader.GetConstantBuffer("PerObjectData");

  m_depthPixelShader.Init(device, "Shaders/PSDepth.sbin");

  D3D11_TEXTURE2D_DESC description;
  description.Width = EngineSettings::ShadowMapSize;
  description.Height = EngineSettings::ShadowMapSize;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_R32_FLOAT;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;
  m_shadowMap.Init(device, description);
  m_shadowMapTarget.Init(device, m_shadowMap);

  description.ArraySize = 6;
  description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
  m_shadowCube.Init(device, description);
  m_shadowCubeTarget.Init(device, m_shadowCube);

  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  description.MiscFlags = 0;
  m_shadowMapDepthBuffer.Init(device, description);
}

DeferredLightPass::~DeferredLightPass()
{
  m_shadowMap.Free();
  m_shadowCube.Free();
}

void DeferredLightPass::Process()
{
  Matrix4 view, camera;
  GraphicsUtils::GetCameraViewMatrix(SceneManager::Instance()->GetScene(), view);
  GraphicsUtils::GetCameraTransformMatrix(SceneManager::Instance()->GetScene(), camera);

  RenderDirectionalLights(view, camera);
  RenderPointLights(view, camera);
  RenderSpotLights(view, camera);
}

void DeferredLightPass::RenderDirectionalLights(Matrix4& view, Matrix4& camera)
{
  const Geometry::Aabb& sceneAabb = SceneManager::Instance()->GetScene().GetAabb();
  Vector3 aabbCorners[8] = {
    Vector3(sceneAabb.m_min[0], sceneAabb.m_min[1], sceneAabb.m_min[2]),
    Vector3(sceneAabb.m_min[0], sceneAabb.m_min[1], sceneAabb.m_max[2]),
    Vector3(sceneAabb.m_min[0], sceneAabb.m_max[1], sceneAabb.m_min[2]),
    Vector3(sceneAabb.m_min[0], sceneAabb.m_max[1], sceneAabb.m_max[2]),
    Vector3(sceneAabb.m_max[0], sceneAabb.m_min[1], sceneAabb.m_min[2]),
    Vector3(sceneAabb.m_max[0], sceneAabb.m_min[1], sceneAabb.m_max[2]),
    Vector3(sceneAabb.m_max[0], sceneAabb.m_max[1], sceneAabb.m_min[2]),
    Vector3(sceneAabb.m_max[0], sceneAabb.m_max[1], sceneAabb.m_max[2])
  };

  DirectionalLightData directionalLightData;
  PerFrameData perFrameData;
  ComponentFactory::ComponentData directionalLights = ComponentFactory::Instance()->GetComponentData<DirectionalLight>();
  for (auto it = directionalLights.begin(); it != directionalLights.end(); ++it)
  {
    DirectionalLight *directionalLight = static_cast<DirectionalLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(directionalLight->GetOwner());

    PrepShadowMap(m_shadowMapTarget);

    if (directionalLight->GetCastsShadows())
    {
      const Vector3& right = transform->GetWorldRight();
      const Vector3& up = transform->GetWorldUp();
      const Vector3& forward = transform->GetWorldForward();
      perFrameData.ViewMatrix.SetViewMatrix(right, up, forward, Vector3(0.0f, 0.0f, 0.0f));

      // Find best view for shadow map
      Vector3 minViewport = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
      Vector3 maxViewport = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
      for (UINT32 i = 0; i < 8; ++i)
      {
        Vector3 transformed = perFrameData.ViewMatrix * aabbCorners[i];
        minViewport[0] = std::min(minViewport[0], transformed[0]);
        minViewport[1] = std::min(minViewport[1], transformed[1]);
        minViewport[2] = std::min(minViewport[2], transformed[2]);
        maxViewport[0] = std::max(maxViewport[0], transformed[0]);
        maxViewport[1] = std::max(maxViewport[1], transformed[1]);
        maxViewport[2] = std::max(maxViewport[2], transformed[2]);
      }

      Vector3 size = maxViewport - minViewport;
      size[0] = size[1] = std::max(size[0], size[1]);

      Vector3 center = (minViewport + maxViewport) / 2.0f;

      directionalLightData.Position = right * center[0] + up * center[1] + forward * (minViewport[2] - 0.1f);
      perFrameData.ViewMatrix.SetViewMatrix(right, up, forward, directionalLightData.Position);
      perFrameData.ProjMatrix.SetOrthographicLH(size[0], size[1], 0.1f, size[2] + 0.1f);
      m_perFrameData->SetData(m_context, perFrameData);
      GraphicsUtils::RenderWorld(m_context, nullptr, m_perObjectData);
      directionalLightData.Position = view * directionalLightData.Position;
      directionalLightData.ShadowMapTransform = perFrameData.ProjMatrix * perFrameData.ViewMatrix * camera;
    }

    PrepLightPass(&m_directionalLightShader, &m_shadowMap, m_dlShadowMapRegister);
    directionalLightData.Direction = view.TransformNormal(transform->GetWorldForward());
    directionalLightData.Color = directionalLight->GetColor();
    directionalLightData.Intensity = directionalLight->GetIntensity();
    directionalLightData.CastsShadows = directionalLight->GetCastsShadows();
    m_directionalLightData->SetData(m_context, directionalLightData);

    GraphicsUtils::RenderFullscreenQuad(m_context);
  }
}

void DeferredLightPass::RenderPointLights(Matrix4& view, Matrix4& camera)
{
  static const Vector3 rights[6] = { -Vector3::c_zAxis, Vector3::c_zAxis, Vector3::c_xAxis, Vector3::c_xAxis, Vector3::c_xAxis, -Vector3::c_xAxis };
  static const Vector3 ups[6] = { Vector3::c_yAxis, Vector3::c_yAxis, -Vector3::c_zAxis, Vector3::c_zAxis, Vector3::c_yAxis, Vector3::c_yAxis };
  static const Vector3 forwards[6] = { Vector3::c_xAxis, -Vector3::c_xAxis, Vector3::c_yAxis, -Vector3::c_yAxis, Vector3::c_zAxis, -Vector3::c_zAxis };

  PointLightData pointLightData;
  PerFrameData perFrameData;
  ComponentFactory::ComponentData pointLights = ComponentFactory::Instance()->GetComponentData<PointLight>();
  for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
  {
    PointLight *pointLight = static_cast<PointLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(pointLight->GetOwner());
    Vector3 pos = transform->GetWorldTranslation();

    for (UINT32 i = 0; i < 6; ++i)
    {
      PrepShadowMap(m_shadowCubeTarget, i);

      if (pointLight->GetCastsShadows())
      {
        perFrameData.ViewMatrix.SetViewMatrix(camera.TransformNormal(rights[i]), camera.TransformNormal(ups[i]), camera.TransformNormal(forwards[i]), pos);
        perFrameData.ProjMatrix.SetPerspectiveFovLH((float)EngineSettings::ShadowMapSize, (float)EngineSettings::ShadowMapSize, SF_RADS_PER_DEG * 90.0f, 0.1f, pointLight->GetRange());
        m_perFrameData->SetData(m_context, perFrameData);
        GraphicsUtils::RenderWorld(m_context, nullptr, m_perObjectData);
      }
    }

    PrepLightPass(&m_pointLightPixelShader, &m_shadowCube, m_plShadowMapRegister);
    pointLightData.Position = view * Vector4(pos[0], pos[1], pos[2], 1.0f);
    pointLightData.Color = pointLight->GetColor();
    pointLightData.Range = pointLight->GetRange();
    pointLightData.Intensity = pointLight->GetIntensity();
    pointLightData.CastsShadows = pointLight->GetCastsShadows();
    m_pointLightData->SetData(m_context, pointLightData);

    GraphicsUtils::RenderFullscreenQuad(m_context);
  }
}

void DeferredLightPass::RenderSpotLights(Matrix4& view, Matrix4& camera)
{
  SpotLightData spotLightData;
  PerFrameData perFrameData;
  ComponentFactory::ComponentData spotLights = ComponentFactory::Instance()->GetComponentData<SpotLight>();
  for (auto it = spotLights.begin(); it != spotLights.end(); ++it)
  {
    SpotLight *spotLight = static_cast<SpotLight*>(*it);
    Transform *transform = ComponentFactory::Instance()->GetComponent<Transform>(spotLight->GetOwner());
    Vector3 pos = transform->GetWorldTranslation();

    PrepShadowMap(m_shadowMapTarget);

    if (spotLight->GetCastsShadows())
    {
      perFrameData.ProjMatrix.SetPerspectiveFovLH((float)EngineSettings::ShadowMapSize, (float)EngineSettings::ShadowMapSize, SF_RADS_PER_DEG * spotLight->GetOuterAngle(), 0.1f, spotLight->GetRange());
      perFrameData.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), pos);
      m_perFrameData->SetData(m_context, perFrameData);
      GraphicsUtils::RenderWorld(m_context, nullptr, m_perObjectData);
      spotLightData.ShadowMapTransform = perFrameData.ProjMatrix * perFrameData.ViewMatrix * camera;
    }

    PrepLightPass(&m_spotLightPixelShader, &m_shadowMap, m_slShadowMapRegister);
    spotLightData.Position = view * Vector4(pos[0], pos[1], pos[2], 1.0f);
    spotLightData.Direction = view.TransformNormal(transform->GetWorldForward());
    spotLightData.Color = spotLight->GetColor();
    spotLightData.Range = spotLight->GetRange();
    spotLightData.Intensity = spotLight->GetIntensity();
    spotLightData.InnerAngle = spotLight->GetInnerAngle() * SF_RADS_PER_DEG / 2.0f;
    spotLightData.OuterAngle = spotLight->GetOuterAngle() * SF_RADS_PER_DEG / 2.0f;
    spotLightData.Falloff = spotLight->GetFalloff();
    spotLightData.CastsShadows = spotLight->GetCastsShadows();
    m_spotLightData->SetData(m_context, spotLightData);

    GraphicsUtils::RenderFullscreenQuad(m_context);
  }
}

void DeferredLightPass::PrepShadowMap(RenderTarget& shadowMap, UINT32 index)
{
  m_shadowMapDepthBuffer.Clear(m_context);
  shadowMap.Clear(m_context, index, Vector4(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX));
  shadowMap.Set(m_context, m_shadowMapDepthBuffer, index, 1);

  m_depthVertexShader.Set(m_context);
  m_depthPixelShader.Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_ENABLED);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  BlendState::Set(m_context, BlendState::NO_BLENDING);
}

void DeferredLightPass::PrepLightPass(D3D11PixelShader *pixelShader, Texture2D *shadowMap, int shadowMapSlot)
{
  m_renderTarget->Set(m_context);
  shadowMap->SetPixel(m_context, shadowMapSlot);
  m_gbuffer->SetTexturePixel(m_context, *pixelShader);

  pixelShader->Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);
  BlendState::Set(m_context, BlendState::ADDITIVE);
}

}
