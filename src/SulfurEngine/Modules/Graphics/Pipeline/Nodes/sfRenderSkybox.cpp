/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderSkybox.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   Node for rendering the skybox to the main target

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderSkybox.hpp"

#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

#include "Components/sfTransform.hpp"
#include "Components/sfCamera.hpp"

#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Scene/sfSceneManager.hpp"

#include "Modules/Graphics/State/sfBlendState.hpp"
#include "Modules/Graphics/State/sfDepthState.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"

namespace Sulfur
{

RenderSkybox::RenderSkybox(D3D11Device& device, RenderTarget *renderTarget, DepthBuffer *depthBuffer)
  : RenderNode(device), m_renderTarget(renderTarget), m_depthBuffer(depthBuffer)
{
  m_vertexShader.Init(device, "./Shaders/VSSkybox.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSSkybox.sbin");

  D3D11VertexShader vertexShader;
  vertexShader.Init(device, "./Shaders/VSTest.sbin");

  m_skyboxMesh.Init(device);
  m_skyboxMesh.CreateBox(10.0f, 10.0f, 10.0f);
}

RenderSkybox::~RenderSkybox()
{

}

void RenderSkybox::Process()
{
  CubeMap *skyboxMap = SceneManager::Instance()->GetScene().m_sceneProperties.GetSkybox();
  if (skyboxMap == nullptr) return;

  m_renderTarget->Set(m_context, *m_depthBuffer);

  DepthState::Set(m_context, DepthState::DEPTH_ENABLED);
  BlendState::Set(m_context, BlendState::NO_BLENDING);
  RasterState::Set(m_context, RasterState::FRONT_FACE_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  Scene& scene = SceneManager::Instance()->GetScene();
  SetupCamera(scene);

  skyboxMap->SetPixel(m_context, 0);
  m_skyboxMesh.Draw(m_context);
}

void RenderSkybox::SetupCamera(Scene& scene)
{
  HNDL objHandle = scene.GetCameraObject();

  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    Camera *camera = object->GetComponent<Camera>();
    const Vector3& translation = transform->GetWorldTranslation();

    PerFrameData perFrame;
    perFrame.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), translation);
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, camera->GetFieldOfView() * SF_RADS_PER_DEG, camera->GetNearPlane(), camera->GetFarPlane());
    perFrame.ViewPosition = transform->GetWorldTranslation();
    m_perFrameData->SetData(m_context, perFrame);

    PerObjectData perObject;
    perObject.WorldMatrix.SetTranslation(translation[0], translation[1], translation[2]);
    m_perObjectData->SetData(m_context, perObject);
  }
  else
  {
    PerFrameData perFrame;
    perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, 3.14159f / 4.0f, 0.1f, 1000.0f);
    perFrame.ViewPosition = Vector3(0.0f, 0.0f, 0.0f);
    m_perFrameData->SetData(m_context, perFrame);

    PerObjectData perObject;
    perObject.WorldMatrix.SetTranslation(0.0f, 0.0f, 0.0f);
    m_perObjectData->SetData(m_context, perObject);
  }
}

}
