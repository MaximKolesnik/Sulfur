/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderSkybox.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   Node for rendering the skybox to the main target

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderSkybox.hpp"

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

RenderSkybox::RenderSkybox(D3D11Device& device, RenderTarget *renderTarget)
  : RenderNode(device), m_renderTarget(renderTarget)
{
  m_vertexShader.Init(device, "./Shaders/VSSkybox.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSSkybox.sbin");

  D3D11VertexShader vertexShader;
  vertexShader.Init(device, "./Shaders/VSTest.sbin");

  m_skyboxMesh.Init(device);
  m_skyboxMesh.CreateBox(10.0f, 10.0f, 10.0f);

  m_skyboxMap = SF_RESOURCE_MANAGER(CubeMap)->LoadResource("Cubemaps/SkyboxSun5deg.dds");
}

RenderSkybox::~RenderSkybox()
{

}

void RenderSkybox::Process()
{
  m_renderTarget->Clear(m_context);
  m_renderTarget->Set(m_context);

  DepthState::Set(m_context, DepthState::DEPTH_DISABLED);
  BlendState::Set(m_context, BlendState::NO_BLENDING);
  RasterState::Set(m_context, RasterState::FRONT_FACE_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  Scene& scene = SceneManager::Instance()->GetScene();
  SetupCamera(scene);

  m_skyboxMap->SetPixel(m_context, 0);
  m_skyboxMesh.Draw(m_context);
}

void RenderSkybox::SetupCamera(Scene& scene)
{
  Object *object = ObjectFactory::Instance()->GetObject(scene.GetCameraObject());
  Transform *transform = object->GetComponent<Transform>();
  Camera *camera = object->GetComponent<Camera>();

  const Vector3& translation = transform->GetTranslation();

  PerFrameData perFrame;
  perFrame.ViewMatrix.SetViewMatrix(transform->GetRight(), transform->GetUp(), transform->GetForward(), transform->GetTranslation());
  perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, camera->GetFieldOfView(), camera->GetNearPlane(), camera->GetFarPlane());
  m_perFrameData->SetData(m_context, perFrame);

  PerObjectData perObject;
  perObject.WorldMatrix.SetTranslation(translation[0], translation[1], translation[2]);
  m_perObjectData->SetData(m_context, perObject);
}

}
