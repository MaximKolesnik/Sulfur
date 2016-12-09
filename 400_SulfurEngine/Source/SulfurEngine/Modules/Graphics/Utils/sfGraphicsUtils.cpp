/******************************************************************************/
/*!
\par     Sulfur
\file    sfGraphicsUtils.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Graphics utility functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGraphicsUtils.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Components/sfCamera.hpp"
#include "Components/sfTransform.hpp"
#include "Components/sfMeshRenderer.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"

namespace Sulfur
{

void GraphicsUtils::Init(D3D11Device& device)
{
  s_fullscreenQuadVertexShader.Init(device, "Shaders/VSFullScreenQuad.sbin");
  s_screenDataConstantBuffer = s_fullscreenQuadVertexShader.GetConstantBuffer("ScreenData");
}

void GraphicsUtils::Free()
{
  s_fullscreenQuadVertexShader.Free();
}

UINT32 GraphicsUtils::DxgiFormatBpp(DXGI_FORMAT format)
{
  switch (format)
  {
  case DXGI_FORMAT_R32G32B32A32_TYPELESS:
  case DXGI_FORMAT_R32G32B32A32_FLOAT:
  case DXGI_FORMAT_R32G32B32A32_UINT:
  case DXGI_FORMAT_R32G32B32A32_SINT:
    return 128;

  case DXGI_FORMAT_R32G32B32_TYPELESS:
  case DXGI_FORMAT_R32G32B32_FLOAT:
  case DXGI_FORMAT_R32G32B32_UINT:
  case DXGI_FORMAT_R32G32B32_SINT:
    return 96;

  case DXGI_FORMAT_R16G16B16A16_TYPELESS:
  case DXGI_FORMAT_R16G16B16A16_FLOAT:
  case DXGI_FORMAT_R16G16B16A16_UNORM:
  case DXGI_FORMAT_R16G16B16A16_UINT:
  case DXGI_FORMAT_R16G16B16A16_SNORM:
  case DXGI_FORMAT_R16G16B16A16_SINT:
  case DXGI_FORMAT_R32G32_TYPELESS:
  case DXGI_FORMAT_R32G32_FLOAT:
  case DXGI_FORMAT_R32G32_UINT:
  case DXGI_FORMAT_R32G32_SINT:
  case DXGI_FORMAT_R32G8X24_TYPELESS:
  case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
  case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
  case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    return 64;

  case DXGI_FORMAT_R10G10B10A2_TYPELESS:
  case DXGI_FORMAT_R10G10B10A2_UNORM:
  case DXGI_FORMAT_R10G10B10A2_UINT:
  case DXGI_FORMAT_R11G11B10_FLOAT:
  case DXGI_FORMAT_R8G8B8A8_TYPELESS:
  case DXGI_FORMAT_R8G8B8A8_UNORM:
  case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
  case DXGI_FORMAT_R8G8B8A8_UINT:
  case DXGI_FORMAT_R8G8B8A8_SNORM:
  case DXGI_FORMAT_R8G8B8A8_SINT:
  case DXGI_FORMAT_R16G16_TYPELESS:
  case DXGI_FORMAT_R16G16_FLOAT:
  case DXGI_FORMAT_R16G16_UNORM:
  case DXGI_FORMAT_R16G16_UINT:
  case DXGI_FORMAT_R16G16_SNORM:
  case DXGI_FORMAT_R16G16_SINT:
  case DXGI_FORMAT_R32_TYPELESS:
  case DXGI_FORMAT_D32_FLOAT:
  case DXGI_FORMAT_R32_FLOAT:
  case DXGI_FORMAT_R32_UINT:
  case DXGI_FORMAT_R32_SINT:
  case DXGI_FORMAT_R24G8_TYPELESS:
  case DXGI_FORMAT_D24_UNORM_S8_UINT:
  case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
  case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
  case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
  case DXGI_FORMAT_R8G8_B8G8_UNORM:
  case DXGI_FORMAT_G8R8_G8B8_UNORM:
  case DXGI_FORMAT_B8G8R8A8_UNORM:
  case DXGI_FORMAT_B8G8R8X8_UNORM:
  case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
  case DXGI_FORMAT_B8G8R8A8_TYPELESS:
  case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
  case DXGI_FORMAT_B8G8R8X8_TYPELESS:
  case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
    return 32;

  case DXGI_FORMAT_R8G8_TYPELESS:
  case DXGI_FORMAT_R8G8_UNORM:
  case DXGI_FORMAT_R8G8_UINT:
  case DXGI_FORMAT_R8G8_SNORM:
  case DXGI_FORMAT_R8G8_SINT:
  case DXGI_FORMAT_R16_TYPELESS:
  case DXGI_FORMAT_R16_FLOAT:
  case DXGI_FORMAT_D16_UNORM:
  case DXGI_FORMAT_R16_UNORM:
  case DXGI_FORMAT_R16_UINT:
  case DXGI_FORMAT_R16_SNORM:
  case DXGI_FORMAT_R16_SINT:
  case DXGI_FORMAT_B5G6R5_UNORM:
  case DXGI_FORMAT_B5G5R5A1_UNORM:
    return 16;

  case DXGI_FORMAT_R8_TYPELESS:
  case DXGI_FORMAT_R8_UNORM:
  case DXGI_FORMAT_R8_UINT:
  case DXGI_FORMAT_R8_SNORM:
  case DXGI_FORMAT_R8_SINT:
  case DXGI_FORMAT_A8_UNORM:
    return 8;

  case DXGI_FORMAT_R1_UNORM:
    return 1;

  case DXGI_FORMAT_BC1_TYPELESS:
  case DXGI_FORMAT_BC1_UNORM:
  case DXGI_FORMAT_BC1_UNORM_SRGB:
  case DXGI_FORMAT_BC4_TYPELESS:
  case DXGI_FORMAT_BC4_UNORM:
  case DXGI_FORMAT_BC4_SNORM:
    return 4;

  case DXGI_FORMAT_BC2_TYPELESS:
  case DXGI_FORMAT_BC2_UNORM:
  case DXGI_FORMAT_BC2_UNORM_SRGB:
  case DXGI_FORMAT_BC3_TYPELESS:
  case DXGI_FORMAT_BC3_UNORM:
  case DXGI_FORMAT_BC3_UNORM_SRGB:
  case DXGI_FORMAT_BC5_TYPELESS:
  case DXGI_FORMAT_BC5_UNORM:
  case DXGI_FORMAT_BC5_SNORM:
  case DXGI_FORMAT_BC6H_TYPELESS:
  case DXGI_FORMAT_BC6H_UF16:
  case DXGI_FORMAT_BC6H_SF16:
  case DXGI_FORMAT_BC7_TYPELESS:
  case DXGI_FORMAT_BC7_UNORM:
  case DXGI_FORMAT_BC7_UNORM_SRGB:
    return 8;

  case DXGI_FORMAT_B4G4R4A4_UNORM:
    return 16;
  }

  return 0;
}

DXGI_FORMAT GraphicsUtils::DxgiFormatFromTypeAndMask(D3D_REGISTER_COMPONENT_TYPE type, UINT32 mask)
{
  switch (type)
  {

  case D3D_REGISTER_COMPONENT_UINT32:
    if (mask == 1)  return DXGI_FORMAT_R32_UINT;
    if (mask <= 3)  return DXGI_FORMAT_R32G32_UINT;
    if (mask <= 7)  return DXGI_FORMAT_R32G32B32_UINT;
    if (mask <= 15) return DXGI_FORMAT_R32G32B32A32_UINT;
    break;

  case D3D_REGISTER_COMPONENT_SINT32:
    if (mask == 1)  return DXGI_FORMAT_R32_SINT;
    if (mask <= 3)  return DXGI_FORMAT_R32G32_SINT;
    if (mask <= 7)  return DXGI_FORMAT_R32G32B32_SINT;
    if (mask <= 15) return DXGI_FORMAT_R32G32B32A32_SINT;
    break;

  case D3D_REGISTER_COMPONENT_FLOAT32:
    if (mask == 1)  return DXGI_FORMAT_R32_FLOAT;
    if (mask <= 3)  return DXGI_FORMAT_R32G32_FLOAT;
    if (mask <= 7)  return DXGI_FORMAT_R32G32B32_FLOAT;
    if (mask <= 15) return DXGI_FORMAT_R32G32B32A32_FLOAT;
    break;

  }

  return DXGI_FORMAT_UNKNOWN;
}

void GraphicsUtils::GetCameraTransformMatrix(Scene& scene, Matrix4& trans)
{
  HNDL objHandle = scene.GetCameraObject();
  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    trans = transform->GetWorldMatrix();
  }
  else
    trans.SetIdentity();
}

void GraphicsUtils::GetCameraViewMatrix(Scene& scene, Matrix4& view)
{
  HNDL objHandle = scene.GetCameraObject();
  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    view.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), transform->GetWorldTranslation());
  }
  else
    view.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
}

void GraphicsUtils::GetProjectionMatrixFromScene(Scene& scene, Real width, Real height, Matrix4& proj)
{
  HNDL objHandle = scene.GetCameraObject();
  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Camera *camera = object->GetComponent<Camera>();
    proj.SetPerspectiveFovLH(width, height, camera->GetFieldOfView() * SF_RADS_PER_DEG, camera->GetNearPlane(), camera->GetFarPlane());
  }
  else
    proj.SetPerspectiveFovLH(width, height, 3.14159f / 4.0f, 0.1f, 1000.0f);
}

void GraphicsUtils::SetupCamera(D3D11Context& context, Real width, Real height, Scene& scene, D3D11ConstantBuffer *perFrameBuffer)
{
  HNDL objHandle = scene.GetCameraObject();

  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    Camera *camera = object->GetComponent<Camera>();

    PerFrameData perFrame;
    perFrame.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), transform->GetWorldTranslation());
    perFrame.ProjMatrix.SetPerspectiveFovLH(width, height, camera->GetFieldOfView() * SF_RADS_PER_DEG, camera->GetNearPlane(), camera->GetFarPlane());
    perFrame.ViewPosition = transform->GetWorldTranslation();
    perFrameBuffer->SetData(context, perFrame);
  }
  else
  {
    PerFrameData perFrame;
    perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
    perFrame.ProjMatrix.SetPerspectiveFovLH(width, height, 3.14159f / 4.0f, 0.1f, 1000.0f);
    perFrame.ViewPosition = Vector3(0.0f, 0.0f, 0.0f);
    perFrameBuffer->SetData(context, perFrame);
  }
}

void GraphicsUtils::RenderWorld(D3D11Context& context, D3D11ConstantBuffer *materialBuffer, D3D11ConstantBuffer *perObjectBuffer)
{
  ComponentFactory::ComponentData componentData = SF_GET_COMP_DATA(MeshRenderer);
  for (auto it = componentData.begin(); it != componentData.end(); ++it)
    RenderMeshRenderer(context, static_cast<MeshRenderer*>(*it), materialBuffer, perObjectBuffer);
}

void GraphicsUtils::RenderMeshRenderer(D3D11Context& context, MeshRenderer *meshRenderer, D3D11ConstantBuffer *materialBuffer, D3D11ConstantBuffer *perObjectBuffer)
{
  Mesh *mesh = meshRenderer->GetMesh();

  if (mesh != nullptr)
  {
    Object *object = SF_GET_OBJECT(meshRenderer->GetOwner());
    Transform *transform = object->GetComponent<Transform>();

    if (materialBuffer)
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
      materialBuffer->SetData(context, materialData);

      if (materialData.UsesDiffuseTexture != 0) diffuseTexture->SetPixel(context, 0);
      if (materialData.UsesNormalTexture != 0) normalTexture->SetPixel(context, 1);
      if (materialData.UsesMaterialTexture != 0) materialTexture->SetPixel(context, 2);
      if (materialData.UsesEmissiveTexture != 0) emissiveTexture->SetPixel(context, 3);
    }

    PerObjectData perObject;
    perObject.WorldMatrix = transform->GetWorldMatrix();
    perObjectBuffer->SetData(context, perObject);

    mesh->Draw(context);
  }
}

D3D11VertexShader GraphicsUtils::s_fullscreenQuadVertexShader;
D3D11ConstantBuffer *GraphicsUtils::s_screenDataConstantBuffer = nullptr;

void GraphicsUtils::RenderFullscreenQuad(D3D11Context& context)
{
  s_fullscreenQuadVertexShader.Set(context);
  context.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
  RasterState::Set(context, RasterState::BACK_FACE_CULLING);
  context.Draw(4, 0);
}

void GraphicsUtils::RenderFullscreenQuad(D3D11Context& context, Scene& scene, Real width, Real height)
{
  HNDL objHandle = scene.GetCameraObject();
  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Camera *camera = object->GetComponent<Camera>();
    RenderFullscreenQuad(context, camera->GetFieldOfView() * SF_RADS_PER_DEG, width, height);
  }
  else
  {
    RenderFullscreenQuad(context, 3.14159f / 4.0f, width, height);
  }
}

void GraphicsUtils::RenderFullscreenQuad(D3D11Context& context, Real fov, Real width, Real height)
{
  Real hH = tan(fov / 2.0f);
  Real hW = hH * width / height;
  Vector3 frustum[4] = {
    Vector3(-hW, hH, 1.0f),
    Vector3(hW, hH, 1.0f),
    Vector3(-hW, -hH, 1.0f),
    Vector3(hW, -hH, 1.0f)
  };

  s_screenDataConstantBuffer->SetData(context, frustum, 4);
  RenderFullscreenQuad(context);
}

}
