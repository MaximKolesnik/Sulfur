/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderGbuffer.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the scene to a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderGbuffer.hpp"

#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

#include "Components/sfTransform.hpp"
#include "Components/sfCamera.hpp"

#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Scene/sfSceneManager.hpp"

#include "Modules/Graphics/State/sfBlendState.hpp"
#include "Modules/Graphics/State/sfDepthState.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"

namespace Sulfur
{

RenderGbuffer::RenderGbuffer(D3D11Device& device, RenderTarget *gBuffer, DepthBuffer *depthBuffer)
  : RenderNode(device), m_gBuffer(gBuffer), m_depthBuffer(depthBuffer)
{
  m_vertexShader.Init(device, "./Shaders/VSDeferredPass.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSDeferredPass.sbin");
  m_materialData = m_pixelShader.GetConstantBuffer("MaterialData");

  m_defaultTexture = SF_RESOURCE_MANAGER(Texture2D)->LoadResource("Images/default.png");
}

RenderGbuffer::~RenderGbuffer()
{
  m_vertexShader.Free();
  m_pixelShader.Free();
}

void RenderGbuffer::Process()
{
  m_gBuffer->Clear(m_context, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  m_depthBuffer->Clear(m_context);
  m_gBuffer->Set(m_context, *m_depthBuffer);

  DepthState::Set(m_context, DepthState::DEPTH_ENABLED);
  BlendState::Set(m_context, BlendState::NO_BLENDING);
  RasterState::Set(m_context, RasterState::BACK_FACE_CULLING);
  SamplerState::SetPixel(m_context, SamplerState::LINEAR, 0);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  Scene& scene = SceneManager::Instance()->GetScene();
  GraphicsUtils::SetupCamera(m_context, (Real)m_gBuffer->GetTexture()->GetDescription().Width, (Real)m_gBuffer->GetTexture()->GetDescription().Height, scene, m_perFrameData);
  GraphicsUtils::RenderWorld(m_context, m_materialData, m_perObjectData);
}

}
