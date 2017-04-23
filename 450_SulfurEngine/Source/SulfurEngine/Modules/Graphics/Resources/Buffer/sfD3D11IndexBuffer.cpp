/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11IndexBuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Index Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11IndexBuffer.hpp"

namespace Sulfur
{

void D3D11IndexBuffer::Init(D3D11Device& device, UINT32 indexCount, const UINT32 *indices)
{
  D3D11_BUFFER_DESC description;
  description.ByteWidth = sizeof(UINT32) * indexCount;
  description.Usage = indices == nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_INDEX_BUFFER;
  description.CPUAccessFlags = indices == nullptr ? D3D11_CPU_ACCESS_WRITE : 0;
  description.MiscFlags = 0;
  description.StructureByteStride = sizeof(UINT32);

  D3D11Buffer::Init(device, description, indices);
}

void D3D11IndexBuffer::Set(D3D11Context& context)
{
  context.GetD3DResource()->IASetIndexBuffer(m_resource, DXGI_FORMAT_R32_UINT, 0);
}

}
