/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Buffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Buffer.hpp"

namespace Sulfur
{

void D3D11Buffer::Init(D3D11Device& device, const D3D11_BUFFER_DESC& description, const void *initialData)
{
  D3D11_SUBRESOURCE_DATA srd;
  srd.pSysMem = initialData;
  srd.SysMemPitch = 0;
  srd.SysMemSlicePitch = 0;

  ID3D11Buffer *buffer = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateBuffer(&description, initialData == nullptr ? nullptr : &srd, &buffer),
    "Failed to create buffer."
    );

  WrapperBase::Init(buffer);
}

void D3D11Buffer::SetData(D3D11Context& context, const void *data, UINT32 dataSize)
{
  D3D11_MAPPED_SUBRESOURCE msr;
  context.GetD3DResource()->Map(m_resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
  memcpy(msr.pData, data, dataSize);
  context.GetD3DResource()->Unmap(m_resource, 0);
}

}
