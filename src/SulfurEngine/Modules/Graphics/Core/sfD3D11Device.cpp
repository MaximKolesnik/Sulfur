/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Device.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 device wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Device.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"

namespace Sulfur
{

void D3D11Device::Init(D3D11Adapter& adapter)
{
  const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
  UINT32 flags = 0;
  ID3D11Device *device = nullptr;
  ID3D11DeviceContext *immediateContext = nullptr;

  // Enable debug support
#ifdef _DEBUG
  flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  HRESULT result = D3D11CreateDevice(
    adapter.GetD3DResource(),
    D3D_DRIVER_TYPE_UNKNOWN,
    nullptr,
    flags,
    featureLevels,
    sizeof(featureLevels) / sizeof(featureLevels[0]),
    D3D11_SDK_VERSION,
    &device,
    &m_featureLevel,
    &immediateContext
    );

  SF_CRITICAL_ERR_HRESULT(
    result,
    "Failed to create Direct3D 11 Device."
    );

  WrapperBase::Init(device);
  m_immediateContext.Init(immediateContext);

  SF_LOG_MESSAGE("Direct3D 11 Device Created");
  std::wstring cardDescriptionW = adapter.GetDescription().Description;
  SF_LOG_MESSAGE(std::string(cardDescriptionW.begin(), cardDescriptionW.end()));

  Real dedicatedMemory = adapter.GetDescription().DedicatedVideoMemory / 1073741824.0f;
  Real sharedMemory = adapter.GetDescription().DedicatedSystemMemory / 1073741824.0f;
  char memoryDescription[1024];
  sprintf_s(memoryDescription, "Dedicated VRAM: %.2f GiB\tShared VRAM: %.2f GiB", dedicatedMemory, sharedMemory);
  SF_LOG_MESSAGE(memoryDescription);
}

void D3D11Device::Free()
{
  WrapperBase::Free();
  m_immediateContext.Free();
}

D3D11Context D3D11Device::GetImmediateContext() const
{
  return m_immediateContext;
}

}
