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
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"

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

  Scene& scene = SceneManager::Instance()->GetScene();
  GraphicsUtils::SetupCamera(m_context, (Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, scene, m_perFrameData);

  PerObjectData perObject;
  perObject.WorldMatrix.SetIdentity();
  m_perObjectData->SetData(m_context, perObject);

  DebugDraw::Instance()->Draw(m_context);

  m_wireframeShader.Set(m_context);
  DebugDraw::Instance()->DrawWireframe(m_context, m_perObjectData);
}

}
