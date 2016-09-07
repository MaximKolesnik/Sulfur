/******************************************************************************/
/*!
\par     Sulfur
\file    sfMesh.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Mesh

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11VertexBuffer.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11IndexBuffer.hpp"

namespace Sulfur
{
	
  class Mesh
  {
  
  public:
    void Init(D3D11Device& device);
    void Free();

    void Draw(D3D11Context& context);

    void AddVertex(const Vertex& vertex);
    void AddIndex(UINT32 index);
    void AddLine(UINT32 i0, UINT32 i1);
    void AddTriangle(UINT32 i0, UINT32 i1, UINT32 i2);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

    void CreateCube(Real size);
    void CreateBox(Real w, Real h, Real d);
    void CreateCubeOutline(Real size);
    void CreateBoxOutline(Real w, Real h, Real d);
    void CreateVector(Real length, Real arrowSize);

  private:
    void UpdateBuffers();

  private:
    bool m_buffersCreated;
    bool m_valid;

    std::vector<Vertex> m_vertices;
    std::vector<UINT32> m_indices;

    D3D11Device *m_device;
    D3D11_PRIMITIVE_TOPOLOGY m_topology;
    D3D11VertexBuffer m_vertexBuffer;
    D3D11IndexBuffer m_indexBuffer;

  };
  
}