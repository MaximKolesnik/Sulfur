/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderGbuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the scene to a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderGbuffer.hpp"

#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "Components/sfTransform.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

#include "Modules/Resource/sfResourceManager.hpp"

namespace Sulfur
{

RenderGbuffer::RenderGbuffer(D3D11Device& device, RenderTarget *renderTarget)
  : RenderNode(device), m_renderTarget(renderTarget)
{
  m_vertexShader.Init(device, "./Shaders/VSTest.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSTexture.sbin");

  D3D11VertexShader vertexShader;
  vertexShader.Init(device, "./Shaders/VSTest.sbin");

  m_texture = SF_RESOURCE_MANAGER(Texture2D)->LoadResource("Images/default.png");
  m_boxMesh = SF_RESOURCE_MANAGER(Mesh)->LoadResource("Models/cube.fbx");
}

RenderGbuffer::~RenderGbuffer()
{

}

void RenderGbuffer::Process()
{
  static Real rx = 0.0f, ry = 0.0f, rz = 0.0f, ct = 0.0f;
  rx += 0.0001f;
  ry += 0.0002f;
  rz += 0.0001f;
  ct += 0.0001f;

  m_renderTarget->Clear(m_context);
  m_renderTarget->Set(m_context);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  m_texture->SetPixel(m_context, 0);

  PerFrameData perFrame;
  perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
  perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, 3.14159f / 4.0f, 0.1f, 1000.0f);
  m_perFrameData->SetData(m_context, perFrame);

  ComponentFactory::ComponentData componentData = ComponentFactory::Instance()->GetComponentData<MeshRenderer>();
  for (auto it = componentData.begin(); it != componentData.end(); ++it)
    RenderMeshRenderer(static_cast<MeshRenderer*>(*it));
}


void RenderGbuffer::RenderMeshRenderer(MeshRenderer *meshRenderer)
{
  Mesh *mesh = meshRenderer->GetMesh();

  if (mesh != nullptr)
  {
    Object *object = ObjectFactory::Instance()->GetObject(meshRenderer->GetOwner());
    Transform* transform = object->GetComponent<Transform>();
    //transform->Update();

    PerObjectData perObject;
    perObject.WorldMatrix = transform->GetWorldMatrix();
    m_perObjectData->SetData(m_context, perObject);

    mesh->Draw(m_context);
  }
}

}
