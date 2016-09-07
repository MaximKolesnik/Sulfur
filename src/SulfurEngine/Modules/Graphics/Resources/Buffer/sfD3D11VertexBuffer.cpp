/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11VertexBuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Vertex Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11VertexBuffer.hpp"

namespace Sulfur
{

void D3D11VertexBuffer::Init(D3D11Device& device, UINT32 vertexSize, UINT32 vertexCount, const void *vertexData)
{
  m_vertexSize = vertexSize;

  D3D11_BUFFER_DESC description;
  description.ByteWidth = vertexSize * vertexCount;
  description.Usage = vertexData == nullptr ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  description.CPUAccessFlags = vertexData == nullptr ? D3D11_CPU_ACCESS_WRITE : 0;
  description.MiscFlags = 0;
  description.StructureByteStride = vertexSize;

  D3D11Buffer::Init(device, description, vertexData);
}

void D3D11VertexBuffer::Set(D3D11Context& context, UINT32 slot)
{
  const UINT32 offset = 0;
  context.GetD3DResource()->IASetVertexBuffers(slot, 1, &m_resource, &m_vertexSize, &offset);
}

}
