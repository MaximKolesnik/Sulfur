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

RenderGbuffer::RenderGbuffer(D3D11Device& device, RenderTarget *gBuffer, DepthBuffer *depthBuffer)
  : RenderNode(device), m_gBuffer(gBuffer), m_depthBuffer(depthBuffer)
{
  m_vertexShader.Init(device, "./Shaders/VSDeferredPass.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSDeferredPass.sbin");
  m_materialData = m_pixelShader.GetConstantBuffer("MaterialData");

  m_defaultTexture = SF_RESOURCE_MANAGER(Texture2D)->LoadResource("Images/default.png");
}

RenderGbuffer::~RenderGbuffer()
{
  m_vertexShader.Free();
  m_pixelShader.Free();
}

void RenderGbuffer::Process()
{
  m_gBuffer->Clear(m_context, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  m_depthBuffer->Clear(m_context);
  m_gBuffer->Set(m_context, *m_depthBuffer);

  DepthState::Set(m_context, DepthState::DEPTH_ENABLED);
  BlendState::Set(m_context, BlendState::NO_BLENDING);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  Scene& scene = g_SystemTable->SceneManager->GetScene();
  SetupCamera(scene);  

  ComponentFactory::ComponentData componentData = g_SystemTable->CompFactory->GetComponentData<MeshRenderer>();
  for (auto it = componentData.begin(); it != componentData.end(); ++it)
    RenderMeshRenderer(static_cast<MeshRenderer*>(*it));
}

void RenderGbuffer::SetupCamera(Scene& scene)
{
  Object *object = g_SystemTable->ObjFactory->GetObject(scene.GetCameraObject());
  Transform *transform = object->GetComponent<Transform>();
  Camera *camera = object->GetComponent<Camera>();

  PerFrameData perFrame;
  perFrame.ViewMatrix.SetViewMatrix(transform->GetRight(), transform->GetUp(), transform->GetForward(), transform->GetTranslation());
  perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_gBuffer->GetTexture()->GetDescription().Width, (Real)m_gBuffer->GetTexture()->GetDescription().Height, camera->GetFieldOfView(), camera->GetNearPlane(), camera->GetFarPlane());
  perFrame.ViewPosition = transform->GetTranslation();
  m_perFrameData->SetData(m_context, perFrame);
}

void RenderGbuffer::RenderMeshRenderer(MeshRenderer *meshRenderer)
{
  Mesh *mesh = meshRenderer->GetMesh();

  if (mesh != nullptr)
  {
    const Material& material = meshRenderer->GetMaterial();
    Texture2D *diffuseTexture = material.GetDiffuseTexture();
    Texture2D *normalTexture = material.GetNormalTexture();
    Texture2D *materialTexture = material.GetMaterialTexture();
    Texture2D *emissiveTexture = material.GetEmissiveTexture();

    MaterialData materialData;
    materialData.DiffuseColor = material.GetDiffuseColor();
    materialData.EmissiveColor = material.GetEmissiveColor();
    materialData.UsesDiffuseTexture = diffuseTexture != nullptr;
    materialData.UsesNormalTexture = normalTexture != nullptr;
    materialData.UsesMaterialTexture = materialTexture != nullptr;
    materialData.UsesEmissiveTexture = emissiveTexture != nullptr;
    materialData.Metallic = material.GetMetallic();
    materialData.Roughness = material.GetRoughness();
    m_materialData->SetData(m_context, materialData);

    if (materialData.UsesDiffuseTexture != 0) diffuseTexture->SetPixel(m_context, 0);
    if (materialData.UsesNormalTexture != 0) normalTexture->SetPixel(m_context, 1);
    if (materialData.UsesMaterialTexture != 0) materialTexture->SetPixel(m_context, 2);
    if (materialData.UsesEmissiveTexture != 0) emissiveTexture->SetPixel(m_context, 3);

    Object *object = g_SystemTable->ObjFactory->GetObject(meshRenderer->GetOwner());
    Transform* transform = object->GetComponent<Transform>();

    PerObjectData perObject;
    perObject.WorldMatrix = transform->GetWorldMatrix();
    m_perObjectData->SetData(m_context, perObject);

    mesh->Draw(m_context);
  }
}

}
