/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderDebugDraw.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering debug draw objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderDebugDraw.hpp"

#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Components/sfCamera.hpp"

namespace Sulfur
{

RenderDebugDraw::RenderDebugDraw(D3D11Device& device, RenderTarget *renderTarget, DepthBuffer *depthBuffer)
  : RenderNode(device), m_renderTarget(renderTarget), m_depthBuffer(depthBuffer)
{
  m_vertexShader.Init(device, "./Shaders/VSTest.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSTest.sbin");
  m_wireframeShader.Init(device, "Shaders/PSWireframe.sbin");
}

RenderDebugDraw::~RenderDebugDraw()
{

}

void RenderDebugDraw::Process()
{
  m_renderTarget->Set(m_context, *m_depthBuffer);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  SetupCamera(SceneManager::Instance()->GetScene());

  PerObjectData perObject;
  perObject.WorldMatrix.SetIdentity();
  m_perObjectData->SetData(m_context, perObject);

  DebugDraw::Instance()->Draw(m_context);

  m_wireframeShader.Set(m_context);
  DebugDraw::Instance()->DrawWireframe(m_context, m_perObjectData);
}

void RenderDebugDraw::SetupCamera(Scene& scene)
{
  HNDL objHandle = scene.GetCameraObject();

  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    Camera *camera = object->GetComponent<Camera>();

    PerFrameData perFrame;
    perFrame.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), transform->GetWorldTranslation());
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, camera->GetFieldOfView() * SF_RADS_PER_DEG, camera->GetNearPlane(), camera->GetFarPlane());
    perFrame.ViewPosition = transform->GetWorldTranslation();
    m_perFrameData->SetData(m_context, perFrame);
  }
  else
  {
    PerFrameData perFrame;
    perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, 3.14159f / 4.0f, 0.1f, 1000.0f);
    perFrame.ViewPosition = Vector3(0.0f, 0.0f, 0.0f);
    m_perFrameData->SetData(m_context, perFrame);
  }
}

}
