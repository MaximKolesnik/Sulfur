/******************************************************************************/
/*!
\par     Sulfur
\file    sfVrManager.cpp
\author
\par     DP email:
\date    02/25/2017

\brief   Manages OpenVR events

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfVrManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"

namespace Sulfur
{

void VrManager::Init()
{
  vr::EVRInitError error;
  m_system = vr::VR_Init(&error, vr::EVRApplicationType::VRApplication_Scene);

  SF_ASSERT(
    m_system,
    vr::VR_GetVRInitErrorAsEnglishDescription(error)
  );

  m_renderModels = vr::VRRenderModels();
  m_system->CaptureInputFocus();

  for (int i = 0; i < ControllerCount; ++i)
  {
    m_controllers[i].Initialized = false;
  }

  for (int i = 0; i < DeviceCount; ++i)
  {
    m_devices[i].Initialized = false;
  }

  for (int i = 0; i < ControllerCount + DeviceCount; ++i)
  {
    if (m_system->IsTrackedDeviceConnected(i))
      OnDeviceActivated(i);
  }
}

void VrManager::Free()
{
  vr::VR_Shutdown();
  m_system = nullptr;
}

void VrManager::Update()
{
  vr::VREvent_t event;
  while (m_system->PollNextEvent(&event, sizeof(vr::VREvent_t)))
  {
    switch (event.eventType)
    {
    case vr::VREvent_TrackedDeviceActivated:
      OnDeviceActivated(event.trackedDeviceIndex);
      break;

    case vr::VREvent_TrackedDeviceDeactivated:
      OnDeviceDeactivated(event.trackedDeviceIndex);
      break;

    case vr::VREvent_TrackedDeviceUpdated:
      OnDeviceUpdate(event.trackedDeviceIndex);
      break;

    case vr::VREvent_ButtonPress:
    case vr::VREvent_ButtonUnpress:
    case vr::VREvent_ButtonTouch:
    case vr::VREvent_ButtonUntouch:
      UpdateControllers();
      break;
    }
  }
}

bool VrManager::IsControllerConnected(Controller controller) const
{
  return m_controllers[controller].Initialized;
}

Mesh& VrManager::GetControllerMesh(Controller controller)
{
  return m_controllers[controller].Mesh;
}

Texture2D& VrManager::GetControllerTexture(Controller controller)
{
  return m_controllers[controller].Texture;
}

const vr::VRControllerState_t& VrManager::GetControllerState(Controller controller) const
{
  return m_controllers[controller].State;
}

bool VrManager::IsDeviceConnected(Device device) const
{
  return m_devices[device].Initialized;
}

Mesh& VrManager::GetDeviceMesh(Device device)
{
  return m_devices[device].Mesh;
}

Texture2D& VrManager::GetDeviceTexture(Device device)
{
  return m_devices[device].Texture;
}

void VrManager::OnDeviceActivated(vr::TrackedDeviceIndex_t i)
{
  vr::ETrackedDeviceClass deviceClass = m_system->GetTrackedDeviceClass(i);
  if (deviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_HMD)
  {
    if (!m_devices[HEADSET].Initialized)
      InitDevice(&m_devices[HEADSET], i);
  }
  else if (deviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference)
  {
    if (!m_devices[LIGHTHOUSE_1].Initialized && !(m_devices[LIGHTHOUSE_2].Initialized && m_devices[LIGHTHOUSE_2].Index == i))
      InitDevice(&m_devices[LIGHTHOUSE_1], i);
    else if (!m_devices[LIGHTHOUSE_2].Initialized && !(m_devices[LIGHTHOUSE_1].Initialized && m_devices[LIGHTHOUSE_1].Index == i))
      InitDevice(&m_devices[LIGHTHOUSE_2], i);
  }
  else if (deviceClass == vr::ETrackedDeviceClass::TrackedDeviceClass_Controller)
  {
    switch (m_system->GetControllerRoleForTrackedDeviceIndex(i))
    {
    case vr::TrackedControllerRole_LeftHand:
      m_system->GetControllerState(i, &m_controllers[LEFT_HAND].State, sizeof(m_controllers[LEFT_HAND].State));
      InitDevice(&m_controllers[LEFT_HAND], i);
      break;

    case vr::TrackedControllerRole_RightHand:
      m_system->GetControllerState(i, &m_controllers[RIGHT_HAND].State, sizeof(m_controllers[RIGHT_HAND].State));
      InitDevice(&m_controllers[RIGHT_HAND], i);
      break;
    }
  }
}

void VrManager::OnDeviceDeactivated(vr::TrackedDeviceIndex_t index)
{

}

void VrManager::OnDeviceUpdate(vr::TrackedDeviceIndex_t index)
{

}

void VrManager::InitDevice(TrackedDevice *device, vr::TrackedDeviceIndex_t index)
{
  char buffer[1024];
  m_system->GetStringTrackedDeviceProperty(index, vr::Prop_RenderModelName_String, buffer, 1024);

  vr::RenderModel_t *model;
  vr::EVRRenderModelError error;
  while ((error = m_renderModels->LoadRenderModel_Async(buffer, &model)) == vr::VRRenderModelError_Loading);

  if (error == vr::VRRenderModelError_None)
  {
    CreateMesh(model, device->Mesh, device->Texture);
    m_renderModels->FreeRenderModel(model);
    device->Initialized = true;
    device->Index = index;
  }
}

void VrManager::CreateMesh(vr::RenderModel_t *model, Mesh& mesh, Texture2D& texture)
{
  D3D11Device& device = GraphicsManager::Instance()->GetDevice();

  mesh.Init(device);

  for (UINT32 i = 0; i < model->unVertexCount; ++i)
  {
    Vertex vert;
    vert.m_position = Vector4(model->rVertexData[i].vPosition.v[0], model->rVertexData[i].vPosition.v[1], model->rVertexData[i].vPosition.v[2], 1.0f);
    vert.m_normal = Vector3(model->rVertexData[i].vNormal.v[0], model->rVertexData[i].vNormal.v[1], model->rVertexData[i].vNormal.v[2]);
    vert.m_texCoords = Vector2(model->rVertexData[i].rfTextureCoord[0], model->rVertexData[i].rfTextureCoord[1]);
    
    mesh.AddVertex(vert);
  }

  for (UINT32 i = 0; i < model->unTriangleCount; ++i)
  {
    mesh.AddTriangle(model->rIndexData[i * 3], model->rIndexData[i * 3 + 1], model->rIndexData[i * 3 + 2]);
  }

  vr::EVRRenderModelError error;
  vr::RenderModel_TextureMap_t *map;
  while ((error = m_renderModels->LoadTexture_Async(model->diffuseTextureId, &map)) == vr::VRRenderModelError_Loading);

  if (error == vr::VRRenderModelError_None)
  {
    D3D11_TEXTURE2D_DESC description;
    description.Width = map->unWidth;
    description.Height = map->unHeight;
    description.MipLevels = 1;
    description.ArraySize = 1;
    description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.Usage = D3D11_USAGE_DEFAULT;
    description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    description.CPUAccessFlags = 0;
    description.MiscFlags = 0;

    texture.Init(device, description, (const BYTE*)map->rubTextureMapData);
    m_renderModels->FreeTexture(map);
  }

}

void VrManager::UpdateControllers()
{
  for (int i = 0; i < ControllerCount; ++i)
  {
    if (m_controllers[i].Initialized)
    {
      vr::VRControllerState_t state;
      if (m_system->GetControllerState(i, &state, sizeof(state)))
      {
        if (state.unPacketNum != m_controllers[i].State.unPacketNum)
        {
          m_controllers[i].State = state;
        }
      }
    }
  }
}

}
