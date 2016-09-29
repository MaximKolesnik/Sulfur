/*!****************************************************************************
\file    D3D11BufferData.h
\author  Dylan Norris
\date    Sep 1, 2016

\brief   Direct3D 11 buffer structures
******************************************************************************/
#pragma once
#include "Reflection/sfReflection.hpp"
#include "Math/sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"

namespace Sulfur
{

///////////////////////////////////////////////////////////////////////////////
// Vertex buffers
///////////////////////////////////////////////////////////////////////////////

class Vertex
{

public:
  Vertex(const Vector4& position = Vector4(),
    const Vector2& texCoords = Vector2(),
    const Vector4& color = Vector4())
    : m_position(position), m_texCoords(texCoords), m_color(color)
  {
  }

public:
  Vector4 m_position;
  Vector2 m_texCoords;
  Vector4 m_color;

};

///////////////////////////////////////////////////////////////////////////////
// Constant buffers
///////////////////////////////////////////////////////////////////////////////

struct PerFrameData
{
  Matrix4 ViewMatrix;
  Matrix4 ProjMatrix;
};

struct PerObjectData
{
  Matrix4 WorldMatrix;
};

}
