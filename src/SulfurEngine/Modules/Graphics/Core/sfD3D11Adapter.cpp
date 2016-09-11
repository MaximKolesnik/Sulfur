/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Adapter.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D adapter (aka graphics card)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Adapter.hpp"

namespace Sulfur
{

void D3D11Adapter::Init(IDXGIAdapter *adapter)
{
  WrapperBase::Init(adapter);

  // Enum outputs
  IDXGIOutput *dxgiOutput = nullptr;
  UINT32 i = 0;
  while (m_resource->EnumOutputs(i++, &dxgiOutput) != DXGI_ERROR_NOT_FOUND)
  {
    m_outputs.push_back(D3D11Output());
    m_outputs.back().Init(dxgiOutput);
  }
}

void D3D11Adapter::Free()
{
  WrapperBase::Free();
}

const D3D11Adapter::OutputList& D3D11Adapter::GetOutputs() const
{
  return m_outputs;
}

}
