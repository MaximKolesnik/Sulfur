/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11VertexBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Vertex Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Buffer.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class D3D11VertexBuffer : public D3D11Buffer
  {
  
  public:
    void Init(D3D11Device& device, UINT32 vertexSize, UINT32 vertexCount, const void *vertexData = nullptr);

    template <typename VertexType>
    void Init(D3D11Device& device, UINT32 vertexCount, const VertexType *vertices = nullptr)
    {
      Init(device, sizeof(VertexType), vertexCount, (const void*)vertices);
    }

    void Set(D3D11Context& context, UINT32 slot = 0);

  private:
    UINT32 m_vertexSize;

  };
  
}