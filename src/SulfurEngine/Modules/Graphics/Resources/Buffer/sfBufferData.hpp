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
    const Vector3& normal = Vector3(),
    const Vector3& tangent = Vector3(),
    const Vector3& binormal = Vector3(),
    const Vector4& color = Vector4())
    : m_position(position), m_texCoords(texCoords), m_normal(normal), 
    m_tangent(tangent), m_binormal(binormal), m_color(color)
  {
  }

public:
  Vector4 m_position;
  Vector2 m_texCoords;
  Vector3 m_normal;
  Vector3 m_tangent;
  Vector3 m_binormal;
  Vector4 m_color;

};

///////////////////////////////////////////////////////////////////////////////
// Constant buffers
///////////////////////////////////////////////////////////////////////////////

struct PerFrameData
{
  Matrix4 ViewMatrix;
  Matrix4 ProjMatrix;
  Vector3 ViewPosition;
};

struct PerObjectData
{
  Matrix4 WorldMatrix;
};

struct MaterialData
{
  Vector4 DiffuseColor;
  Vector4 EmissiveColor;
  INT32 UsesDiffuseTexture;
  INT32 UsesNormalTexture;
  INT32 UsesMaterialTexture;
  INT32 UsesEmissiveTexture;
  Real Metallic;
  Real Roughness;
};

struct DirectionalLightData
{
  Vector3 Direction;
  Vector4 Color;
  float Intensity;
};

struct PointLightData
{
  Vector4 Position;
  Vector4 Color;
  float Range;
  float Intensity;
};

struct SpotLightData
{
  Vector4 Position;
  Vector3 Direction;
  Vector4 Color;
  float Range;
  float Intensity;
  float InnerAngle;
  float OuterAngle;
  float Falloff;
};

struct ConvolutionData
{
  UINT32 CubeFace;
  UINT32 OutputWidth;
  UINT32 OutputHeight;
  Real PhiRange;
  Real PhiStep;
  Real ThetaRange;
  Real ThetaStep;
};

}
