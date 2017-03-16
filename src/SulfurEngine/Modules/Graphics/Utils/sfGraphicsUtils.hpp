/******************************************************************************/
/*!
\par     Sulfur
\file    sfGraphicsUtils.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Graphics utility functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Scene/sfScene.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Components/sfMeshRenderer.hpp"
#include "Components/sfTrackedController.hpp"

namespace Sulfur
{
	
  class GraphicsUtils
  {

  public:
    static void Init(D3D11Device& device);
    static void Free();

    static UINT32 DxgiFormatBpp(DXGI_FORMAT format);
    static DXGI_FORMAT DxgiFormatFromTypeAndMask(D3D_REGISTER_COMPONENT_TYPE type, UINT32 mask);

    static void GetCameraTransformMatrix(Scene& scene, Matrix4& transform);
    static void GetCameraViewMatrix(Scene& scene, Matrix4& view);
    static void GetProjectionMatrixFromScene(Scene& scene, Real width, Real height, Matrix4& proj);
    static void SetupCamera(D3D11Context& context, Real width, Real height, Scene& scene, D3D11ConstantBuffer *perFrameBuffer);
    static void RenderWorld(D3D11Context& context, D3D11ConstantBuffer *materialBuffer, D3D11ConstantBuffer *perObjectBuffer);
    static void RenderMeshRenderer(D3D11Context& context, MeshRenderer *meshRenderer, D3D11ConstantBuffer *materialBuffer, D3D11ConstantBuffer *perObjectBuffer);
    static void RenderTrackedController(D3D11Context& context, TrackedController *trackedController, D3D11ConstantBuffer *materialBuffer, D3D11ConstantBuffer *perObjectBuffer);

    // Fullscreen Quad
    static D3D11VertexShader s_fullscreenQuadVertexShader;
    static D3D11ConstantBuffer *s_screenDataConstantBuffer;
    static void RenderFullscreenQuad(D3D11Context& context);
    static void RenderFullscreenQuad(D3D11Context& context, Scene& scene, Real width, Real height);
    static void RenderFullscreenQuad(D3D11Context& context, Real fov, Real width, Real height);
  };
  
}