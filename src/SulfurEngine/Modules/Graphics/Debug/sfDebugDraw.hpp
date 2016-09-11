/******************************************************************************/
/*!
\par     Sulfur
\file    sfDebugDraw.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Debug drawing

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.h"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfMatrix4.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11VertexBuffer.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11IndexBuffer.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"

namespace Sulfur
{
	
  class DebugDraw
  {
    SINGLETON(DebugDraw)

  public:
    void Init(D3D11Device& device);
    void Free();

    void DrawBox(const Matrix4& transform, Real w = 1.0f, Real h = 1.0f, Real d = 1.0f, const Vector4& color = DEFAULT_COLOR);
    void DrawSphere(const Matrix4& transform, Real radius = 0.5f, const Vector4& color = DEFAULT_COLOR);
    void DrawVector(const Vector3& origin, const Vector3& direction, const Vector4& color = DEFAULT_COLOR);
    void DrawLine(const Vector3& p0, const Vector3& p1, const Vector4& color = DEFAULT_COLOR);
    void DrawCircle(const Vector3& center, const Vector3& normal, Real radius, const Vector4& color = DEFAULT_COLOR);

    void Draw(D3D11Context& context);

  private:
    static const UINT32 CIRCLE_SIDES = 36;
    static const UINT32 MAX_VERTICES = 4096;
    static const UINT32 MAX_INDICES = MAX_VERTICES << 2;

    static const Vector4 DEFAULT_COLOR;

    D3D11VertexBuffer m_vertexBuffer;
    Vertex m_vertices[MAX_VERTICES];
    UINT32 m_vertexCount;

    D3D11IndexBuffer m_indexBuffer;
    UINT32 m_indices[MAX_INDICES];
    UINT32 m_indexCount;

  };
  
}