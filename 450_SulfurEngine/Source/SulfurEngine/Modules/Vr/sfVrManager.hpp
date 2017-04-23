/******************************************************************************/
/*!
\par     Sulfur
\file    sfVrManager.hpp
\author  
\par     DP email: 
\date    02/25/2017

\brief   Manages OpenVR events

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include <openvr.h>

namespace Sulfur
{
	
class VrManager
{
  SF_SINGLETON_EMPTY(VrManager)

public:
  enum Controller
  {
    LEFT_HAND,
    RIGHT_HAND,
    ControllerCount
  };

  enum Device
  {
    HEADSET,
    LIGHTHOUSE_1,
    LIGHTHOUSE_2,
    DeviceCount
  };

private:
  struct TrackedDevice
  {
    bool Initialized;
    vr::TrackedDeviceIndex_t Index;
    Mesh Mesh;
    Texture2D Texture;
  };

  struct TrackedController : public TrackedDevice
  {
    vr::VRControllerState_t State;
  };

public:
  void Init();
  void Free();

  void Update();

  bool IsControllerConnected(Controller controller) const;
  Mesh& GetControllerMesh(Controller controller);
  Texture2D& GetControllerTexture(Controller controller);
  const vr::VRControllerState_t& GetControllerState(Controller controller) const;

  bool IsDeviceConnected(Device device) const;
  Mesh& GetDeviceMesh(Device device);
  Texture2D& GetDeviceTexture(Device device);

private:
  void OnDeviceActivated(vr::TrackedDeviceIndex_t index);
  void OnDeviceDeactivated(vr::TrackedDeviceIndex_t index);
  void OnDeviceUpdate(vr::TrackedDeviceIndex_t index);
  void InitDevice(TrackedDevice *device, vr::TrackedDeviceIndex_t index);
  void CreateMesh(vr::RenderModel_t *model, Mesh& mesh, Texture2D& texture);
  void UpdateControllers();

private:
  vr::IVRSystem *m_system = nullptr;
  vr::IVRRenderModels *m_renderModels = nullptr;

  TrackedDevice m_devices[DeviceCount];
  TrackedController m_controllers[ControllerCount];

};
  
}