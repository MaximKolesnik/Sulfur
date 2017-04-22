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
#include "Types/sfSingleton.hpp"
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
	
  class D3D11ConstantBuffer;
  class MeshRenderer;

  class DebugDraw
  {
    SF_SINGLETON(DebugDraw)

  private:
    static const UINT32 MAX_VERTICES = 65536 * 2;
    static const UINT32 MAX_INDICES = MAX_VERTICES << 2;

    struct LineBuffers
    {
      D3D11VertexBuffer m_vertexBuffer;
      Vertex m_vertices[MAX_VERTICES];
      UINT32 m_vertexCount;

      D3D11IndexBuffer m_indexBuffer;
      UINT32 m_indices[MAX_INDICES];
      UINT32 m_indexCount;

      LineBuffers() : m_vertexCount(0), m_indexCount(0) {}
    };

  public:

    void Init(D3D11Device& device);
    void Free();

    void DrawBox2D(const Matrix4& transform, Real w = 1.0f, Real h = 1.0f, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawBox(const Matrix4& transform, Real w = 1.0f, Real h = 1.0f, Real d = 1.0f, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawSphere(const Vector3& center, Real radius = 0.5f, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawSphere(const Matrix4& transform, Real radius = 0.5f, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawSphereHorizon(const Vector3& center, Real radius, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawCone(const Vector3& tip, const Vector3& direction, const Vector3& right, const Vector3& up, Real height, Real angle, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawVector(const Vector3& origin, const Vector3& direction, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawLine(const Vector3& p0, const Vector3& p1, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawCircle(const Vector3& center, const Vector3& normal, Real radius, bool ignoreDepth = false, const Vector4& color = DEFAULT_COLOR);
    void DrawWireframe(const MeshRenderer *meshRenderer);

    void Draw(D3D11Context& context);
    void DrawWireframe(D3D11Context& context, D3D11ConstantBuffer *perObjectBuffer);

    bool IsEnabled() const;
    void SetEnabled(bool enabled);

  private:
    static const UINT32 CIRCLE_SIDES = 36;
    static const Vector4 DEFAULT_COLOR;

    bool m_enabled;
    LineBuffers m_lineBuffers[2];
    std::vector<const MeshRenderer *> m_wireFrameRenderers;

  };
  
}