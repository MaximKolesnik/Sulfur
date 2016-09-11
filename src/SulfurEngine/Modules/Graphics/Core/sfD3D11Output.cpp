/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Output.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D output (aka display)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Output.hpp"

namespace Sulfur
{

void D3D11Output::Init(IDXGIOutput *output)
{
  WrapperBase::Init(output);
}

void D3D11Output::Free()
{
  WrapperBase::Free();

}

void D3D11Output::GetDisplayModesForFormat(DXGI_FORMAT format, std::vector<DXGI_MODE_DESC>& modes)
{
  UINT32 modeCount = 0;
  SF_WARNING_HRESULT(
    m_resource->GetDisplayModeList(format, 0, &modeCount, nullptr),
    "Failed to get display modes for output."
    );

  if (modeCount == 0)
    return;

  UINT32 start = (UINT32)modes.size();
  modes.resize(start + modeCount);

  SF_WARNING_HRESULT(
    m_resource->GetDisplayModeList(format, 0, &modeCount, modes.data() + start),
    "Failed to get display modes for output."
    );
}

void D3D11Output::CopyToTexture(ID3D11Texture2D *texture)
{
  IDXGISurface *surface;
  texture->QueryInterface(IID_IDXGISurface, (void**)&surface);
  
  SF_WARNING_HRESULT(
    m_resource->GetDisplaySurfaceData(surface),
    "Failed to copy output surface to texture"
    );
}

void D3D11Output::GetFrameStats(DXGI_FRAME_STATISTICS& stats)
{
  SF_WARNING_HRESULT(
    m_resource->GetFrameStatistics(&stats),
    "Failed to get frame stats for output."
    );
}

void D3D11Output::WaitForVBlank()
{
  SF_WARNING_HRESULT(
    m_resource->WaitForVBlank(),
    "Failed while waiting for v-blank."
    );
}

}
