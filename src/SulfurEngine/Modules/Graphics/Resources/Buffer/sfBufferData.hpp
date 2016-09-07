/*!****************************************************************************
\file    D3D11BufferData.h
\author  Dylan Norris
\date    Sep 1, 2016

\brief   Direct3D 11 buffer structures
******************************************************************************/
#pragma once
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

struct Vertex
{
  Vector4 position;
  Vector2 texCoords;
  Vector4 color;
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
