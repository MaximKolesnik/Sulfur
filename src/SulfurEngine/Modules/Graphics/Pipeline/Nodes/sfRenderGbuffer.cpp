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
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

namespace Sulfur
{

RenderGbuffer::RenderGbuffer(D3D11Device& device, RenderTarget *renderTarget)
  : RenderNode(device), m_renderTarget(renderTarget)
{
  m_vertexShader.Init(device, "./Shaders/VSTest.sbin");
  m_perFrameData = m_vertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_vertexShader.GetConstantBuffer("PerObjectData");

  m_pixelShader.Init(device, "./Shaders/PSTexture.sbin");

  m_boxMesh.Init(device);
  m_boxMesh.CreateCube(1.0f);

  D3D11VertexShader vertexShader;
  vertexShader.Init(device, "./Shaders/VSTest.sbin");

  m_texture.Init(device, "./Images/default.png");
}

RenderGbuffer::~RenderGbuffer()
{

}

void RenderGbuffer::Process()
{
  static Real rx = 0.0f, ry = 0.0f, rz = 0.0f, ct = 0.0f;
  rx += 0.0001f;
  ry += 0.0002f;
  rz += 0.0001f;
  ct += 0.0001f;

  m_renderTarget->Clear(m_context);
  m_renderTarget->Set(m_context);

  m_vertexShader.Set(m_context);
  m_pixelShader.Set(m_context);

  m_texture.SetPixel(m_context, 0);

  PerFrameData perFrame;
  perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
  perFrame.ProjMatrix.SetPerspectiveFovLH((Real)m_renderTarget->GetTexture()->GetDescription().Width, (Real)m_renderTarget->GetTexture()->GetDescription().Height, 3.14159f / 4.0f, 0.1f, 1000.0f);
  m_perFrameData->SetData(m_context, perFrame);

  PerObjectData perObject;
  perObject.WorldMatrix.SetTransformation(Vector3(rx, ry, rz), Vector3(100.0f, 100.0f, 100.0f), Vector3(0.0f, 0.0, 200.0f));
  m_perObjectData->SetData(m_context, perObject);

  DebugDraw::Instance()->DrawSphere(perObject.WorldMatrix, 50.0f);

  DebugDraw::Instance()->DrawBox(perObject.WorldMatrix);

  perObject.WorldMatrix.SetTransformation(Vector3(rx, ry, rz), Vector3(75.0f, 75.0f, 75.0f), Vector3(0.0f, 0.0, 200.0f));
  DebugDraw::Instance()->DrawBox(perObject.WorldMatrix);

  perObject.WorldMatrix.SetTransformation(Vector3(rx, ry, rz), Vector3(50.0f, 50.0f, 50.0f), Vector3(0.0f, 0.0, 200.0f));
  DebugDraw::Instance()->DrawBox(perObject.WorldMatrix);

  perObject.WorldMatrix.SetTransformation(Vector3(rx, ry, rz), Vector3(25.0f, 25.0f, 25.0f), Vector3(0.0f, 0.0, 200.0f));
  DebugDraw::Instance()->DrawBox(perObject.WorldMatrix);

  perObject.WorldMatrix.SetRotationRad(rx, ry, rz);
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(50.0f, 0.0, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(0.0f, 50.0, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(0.0f, 0.0, 50.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(-50.0f, 0.0, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(0.0f, -50.0, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(Vector3(0.0f, 0.0, 200.0f), perObject.WorldMatrix * Vector3(0.0f, 0.0, -50.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));

  m_boxMesh.Draw(m_context);
}

}
