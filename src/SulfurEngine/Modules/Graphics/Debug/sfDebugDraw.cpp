/******************************************************************************/
/*!
\par     Sulfur
\file    sfDebugDraw.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Debug drawing

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDebugDraw.hpp"

namespace Sulfur
{

const Vector4 DebugDraw::DEFAULT_COLOR = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

DebugDraw::DebugDraw()
{
}

DebugDraw::~DebugDraw()
{
}

void DebugDraw::Init(D3D11Device& device)
{
  m_vertexBuffer.Init<Vertex>(device, MAX_VERTICES);
  m_indexBuffer.Init(device, MAX_INDICES);
}

void DebugDraw::Free()
{
  m_vertexBuffer.Free();
  m_indexBuffer.Free();
}

void DebugDraw::DrawBox(const Matrix4& transform, Real w, Real h, Real d, const Vector4& color)
{
  UINT32 startVert = m_vertexCount;
  Real hw = w / 2.0f, hh = h / 2.0f, hd = d / 2.0f;

  m_vertices[m_vertexCount].position = transform * Vector4(-hw, hh, hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(hw, hh, hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(-hw, hh, -hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(hw, hh, -hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(-hw, -hh, hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(hw, -hh, hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(-hw, -hh, -hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = transform * Vector4(hw, -hh, -hd, 1.0f);
  m_vertices[m_vertexCount++].color = color;

  // Top
  m_indices[m_indexCount++] = startVert + 0; m_indices[m_indexCount++] = startVert + 1;
  m_indices[m_indexCount++] = startVert + 2; m_indices[m_indexCount++] = startVert + 3;
  m_indices[m_indexCount++] = startVert + 0; m_indices[m_indexCount++] = startVert + 2;
  m_indices[m_indexCount++] = startVert + 1; m_indices[m_indexCount++] = startVert + 3;

  // Bottom
  m_indices[m_indexCount++] = startVert + 4; m_indices[m_indexCount++] = startVert + 5;
  m_indices[m_indexCount++] = startVert + 6; m_indices[m_indexCount++] = startVert + 7;
  m_indices[m_indexCount++] = startVert + 4; m_indices[m_indexCount++] = startVert + 6;
  m_indices[m_indexCount++] = startVert + 5; m_indices[m_indexCount++] = startVert + 7;

  // Sides
  m_indices[m_indexCount++] = startVert + 0; m_indices[m_indexCount++] = startVert + 4;
  m_indices[m_indexCount++] = startVert + 1; m_indices[m_indexCount++] = startVert + 5;
  m_indices[m_indexCount++] = startVert + 2; m_indices[m_indexCount++] = startVert + 6;
  m_indices[m_indexCount++] = startVert + 3; m_indices[m_indexCount++] = startVert + 7;
}

void DebugDraw::DrawSphere(const Matrix4& transform, Real radius, const Vector4& color)
{
  Vector3 center = transform * Vector3(0.0f, 0.0f, 0.0f);

  // TODO: Matrix needs a transform normal
  Vector4 right = transform * Vector4(1.0f, 0.0f, 0.0f, 0.0f);
  Vector4 up = transform * Vector4(0.0f, 1.0f, 0.0f, 0.0f);
  Vector4 forward = transform * Vector4(0.0f, 0.0f, 1.0f, 0.0f);

  DrawCircle(center, Vector3(right.GetX(), right.GetY(), right.GetZ()), radius, color);
  DrawCircle(center, Vector3(up.GetX(), up.GetY(), up.GetZ()), radius, color);
  DrawCircle(center, Vector3(forward.GetX(), forward.GetY(), forward.GetZ()), radius, color);
}

void DebugDraw::DrawVector(const Vector3& origin, const Vector3& direction, const Vector4& color)
{
  const Real ARROW_SIZE = 5.0f;

  Vector3 forward = direction.Normalized();
  Vector3 right = Vector3(0.0f, 1.0f, 0.0f).Cross(forward);

  if (right.Length() < 0.001f)
    right = Vector3(1.0f, 0.0f, 0.0f).Cross(forward).Normalized();
  else
    right.Normalize();

  Vector3 up = forward.Cross(right).Normalized();

  Vector3 tip = origin + direction;

  DrawLine(origin, tip, color);
  DrawLine(tip, tip - forward * ARROW_SIZE + right * ARROW_SIZE / 2.0f, color);
  DrawLine(tip, tip - forward * ARROW_SIZE - right * ARROW_SIZE / 2.0f, color);
  DrawLine(tip, tip - forward * ARROW_SIZE + up * ARROW_SIZE / 2.0f, color);
  DrawLine(tip, tip - forward * ARROW_SIZE - up * ARROW_SIZE / 2.0f, color);
}

void DebugDraw::DrawLine(const Vector3& p0, const Vector3& p1, const Vector4& color)
{
  UINT32 startVert = m_vertexCount;

  m_vertices[m_vertexCount].position = Vector4(p0.GetX(), p0.GetY(), p0.GetZ(), 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_vertices[m_vertexCount].position = Vector4(p1.GetX(), p1.GetY(), p1.GetZ(), 1.0f);
  m_vertices[m_vertexCount++].color = color;

  m_indices[m_indexCount++] = startVert + 0; m_indices[m_indexCount++] = startVert + 1;
}

void DebugDraw::DrawCircle(const Vector3& center, const Vector3& normal, Real radius, const Vector4& color)
{
  UINT32 startVert = m_vertexCount;

  Vector3 right = Vector3(0.0f, 1.0f, 0.0f).Cross(normal);

  if (right.Length() < 0.001f)
    right = Vector3(1.0f, 0.0f, 0.0f).Cross(normal).Normalized();
  else
    right.Normalize();

  Vector3 up = normal.Cross(right).Normalized();

  Real angle = 0;
  Real inc = 3.14159f / CIRCLE_SIDES * 2.0f;

  for (UINT32 i = 0; i < CIRCLE_SIDES; ++i)
  {
    Vector3 point = center + (MathUtils::Cos(angle) * right + MathUtils::Sin(angle) * up) * radius;
    m_vertices[m_vertexCount].position = Vector4(point.GetX(), point.GetY(), point.GetZ(), 1.0f);
    m_vertices[m_vertexCount++].color = color;
    m_indices[m_indexCount++] = startVert + i;

    if (i == CIRCLE_SIDES - 1)
      m_indices[m_indexCount++] = startVert;
    else
      m_indices[m_indexCount++] = startVert + i + 1;

    angle += inc;
  }
}

void DebugDraw::Draw(D3D11Context& context)
{
  m_vertexBuffer.SetData(context, m_vertices, m_vertexCount);
  m_indexBuffer.SetData(context, m_indices, m_indexCount);

  m_vertexBuffer.Set(context);
  m_indexBuffer.Set(context);

  context.SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  context.DrawIndexed(m_indexCount, 0, 0);

  m_vertexCount = 0;
  m_indexCount = 0;
}

}
