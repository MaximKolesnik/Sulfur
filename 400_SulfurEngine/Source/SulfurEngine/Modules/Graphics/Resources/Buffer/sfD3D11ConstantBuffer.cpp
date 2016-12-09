/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11ConstantBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Constant Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11ConstantBuffer.hpp"

namespace Sulfur
{

void D3D11ConstantBuffer::Init(D3D11Device& device, UINT32 bufferSize, const void *initialData)
{
  D3D11_BUFFER_DESC description;
  description.ByteWidth = bufferSize;
  description.Usage = D3D11_USAGE_DYNAMIC;
  description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  description.MiscFlags = 0;
  description.StructureByteStride = bufferSize;

  D3D11Buffer::Init(device, description, initialData);
}

void D3D11ConstantBuffer::SetVertex(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->VSSetConstantBuffers(slot, 1, &m_resource);
}

void D3D11ConstantBuffer::SetPixel(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->PSSetConstantBuffers(slot, 1, &m_resource);
}

void D3D11ConstantBuffer::SetCompute(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->CSSetConstantBuffers(slot, 1, &m_resource);
}

}
