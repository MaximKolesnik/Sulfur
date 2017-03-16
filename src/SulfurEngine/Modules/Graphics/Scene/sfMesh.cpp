/******************************************************************************/
/*!
\par     Sulfur
\file    sfMesh.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Mesh

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfMesh.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Resource/Importers/Model/sfFbxImporter.hpp"

namespace Sulfur
{

SF_REGISTER_RESOURCE_TYPE(Mesh)
  SF_IMPORTER(FbxImporter, "fbx")
SF_END_REGISTER_RESOURCE_TYPE()

void Mesh::Init(D3D11Device& device)
{
  m_device = &device;
  m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
  m_buffersCreated = false;
}

void Mesh::Free()
{
  m_vertexBuffer.Free();
  m_indexBuffer.Free();
}

void Mesh::CalculateTangentsAndBinormals()
{
  std::vector<Vector3> tangents(m_vertices.size() * 2);
  Vector3 *tan1 = tangents.data();
  Vector3 *tan2 = tangents.data() + m_vertices.size();
  SecureZeroMemory(tan1, sizeof(Vector3) * tangents.size());

  for (UINT32 i = 0; i < m_indices.size(); i+=3)
  {
    UINT32 i1 = m_indices[i];
    UINT32 i2 = m_indices[i + 1];
    UINT32 i3 = m_indices[i + 2];

    const Vertex &v1 = m_vertices[i1];
    const Vertex &v2 = m_vertices[i2];
    const Vertex &v3 = m_vertices[i3];

    float x1 = v2.m_position[0] - v1.m_position[0];
    float x2 = v3.m_position[0] - v1.m_position[0];
    float y1 = v2.m_position[1] - v1.m_position[1];
    float y2 = v3.m_position[1] - v1.m_position[1];
    float z1 = v2.m_position[2] - v1.m_position[2];
    float z2 = v3.m_position[2] - v1.m_position[2];

    float s1 = v2.m_texCoords[0] - v1.m_texCoords[0];
    float s2 = v3.m_texCoords[0] - v1.m_texCoords[0];
    float t1 = v2.m_texCoords[1] - v1.m_texCoords[1];
    float t2 = v3.m_texCoords[1] - v1.m_texCoords[1];

    float r = 1.0f / (s1 * t2 - s2 * t1);
    Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
      (t2 * z1 - t1 * z2) * r);
    Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
      (s1 * z2 - s2 * z1) * r);

    tan1[i1] += sdir;
    tan1[i2] += sdir;
    tan1[i3] += sdir;

    tan2[i1] += tdir;
    tan2[i2] += tdir;
    tan2[i3] += tdir;
  }

  for (UINT32 i = 0; i < m_vertices.size(); ++i)
  {
    const Vector3& normal = m_vertices[i].m_normal;
    const Vector3& tangent = tan1[i];

    m_vertices[i].m_tangent = (tangent - normal * normal.Dot(tangent)).Normalize();
    m_vertices[i].m_tangent = ((normal.Cross(tangent).Dot(tan2[i])) < 0.0f) ? -m_vertices[i].m_tangent : m_vertices[i].m_tangent;

    m_vertices[i].m_binormal = m_vertices[i].m_normal.Cross(m_vertices[i].m_tangent);
  }

  m_valid = false;
}

void Mesh::Draw(D3D11Context& context)
{
  if (!m_valid)
  {
    UpdateBuffers();
    m_valid = true;
  }

  // Set the topology
  context.SetTopology((D3D11_PRIMITIVE_TOPOLOGY)m_topology);

  m_vertexBuffer.Set(context);
  m_indexBuffer.Set(context);

  // Draw
  switch (m_topology)
  {
  case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
  case D3D11_PRIMITIVE_TOPOLOGY_LINELIST:
    context.DrawIndexed((UINT32)m_indices.size(), 0, 0);
    break;

  case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
  case D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP:
    context.Draw((UINT32)m_vertices.size(), 0);
    break;
  }
}

UINT32 Mesh::AddVertex(const Vertex& vertex)
{
  m_vertices.push_back(vertex);
  m_valid = false;
  m_aabb.Expand(Vector3(vertex.m_position[0], vertex.m_position[1], vertex.m_position[2]));

  return (UINT32)m_vertices.size() - 1;
}

UINT32 Mesh::AddUniqueVertex(const Vertex& vertex)
{
  //TODO: Optimizations
  UINT32 vertexCount = (UINT32)m_vertices.size();
  for (UINT32 i = 0; i < vertexCount; ++i)
  {
    if (memcmp(&vertex, &m_vertices[i], sizeof(Vertex)) == 0)
      return i;
  }

  return AddVertex(vertex);
}

void Mesh::AddIndex(UINT32 index)
{
  m_indices.push_back(index);
  m_valid = false;
}

void Mesh::AddLine(UINT32 i0, UINT32 i1)
{
  m_indices.push_back(i0);
  m_indices.push_back(i1);
  m_valid = false;
}

void Mesh::AddTriangle(UINT32 i0, UINT32 i1, UINT32 i2)
{
  m_indices.push_back(i0);
  m_indices.push_back(i1);
  m_indices.push_back(i2);
  m_valid = false;
}

void Mesh::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
  m_topology = topology;
}

void Mesh::CreateCube(Real size)
{
  CreateBox(size, size, size);
}

void Mesh::CreateBox(Real w, Real h, Real d)
{
  Real hw = w / 2.0f, hh = h / 2.0f, hd = d / 2.0f;

  // Top
  AddVertex(Vertex( Vector4(-hw,  hh,  hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw,  hh,  hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw,  hh, -hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw,  hh, -hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) ));
  AddTriangle(0, 1, 2);
  AddTriangle(1, 3, 2);

  // Bottom
  AddVertex(Vertex( Vector4(-hw, -hh,  hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh,  hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw, -hh, -hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh, -hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) ));
  AddTriangle(4, 6, 5);
  AddTriangle(5, 6, 7);

  // Left
  AddVertex(Vertex( Vector4(-hw,  hh,  hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw,  hh, -hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw, -hh,  hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw, -hh, -hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f) ));
  AddTriangle(8, 9, 10);
  AddTriangle(9, 11, 10);

  // Right
  AddVertex(Vertex( Vector4(hw,  hh,  hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw,  hh, -hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh,  hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh, -hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) ));
  AddTriangle(12, 14, 13);
  AddTriangle(13, 14, 15);

  // Front
  AddVertex(Vertex( Vector4(-hw,  hh, -hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw,  hh, -hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw, -hh, -hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh, -hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f) ));
  AddTriangle(16, 17, 18);
  AddTriangle(17, 19, 18);

  // Back
  AddVertex(Vertex( Vector4(-hw,  hh,  hd, 1.0f), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw,  hh,  hd, 1.0f), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(-hw, -hh,  hd, 1.0f), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f) ));
  AddVertex(Vertex( Vector4(hw, -hh,  hd, 1.0f), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f) ));
  AddTriangle(20, 22, 21);
  AddTriangle(21, 22, 23);

  SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

const Geometry::Aabb& Mesh::GetAabb() const
{
  return m_aabb;
}

void Mesh::UpdateBuffers()
{
  if (m_buffersCreated)
  {
    m_vertexBuffer.Free();
    m_indexBuffer.Free();
  }

  m_vertexBuffer.Init(*m_device, (UINT32)m_vertices.size(), m_vertices.data());
  m_indexBuffer.Init(*m_device, (UINT32)m_indices.size(), m_indices.data());
  m_buffersCreated = true;
}

}
