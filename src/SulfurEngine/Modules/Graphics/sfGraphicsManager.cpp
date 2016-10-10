/****************************************************************************** /
/*!
\par     Sulfur
\file    sfGraphicsManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Graphics Manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGraphicsManager.hpp"
#include "Core/sfD3D11Factory.hpp"
#include "Pipeline/Pipelines/sfDeferredPipeline.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "SystemTable/sfSystemTable.hpp"

// States
#include "State/sfBlendState.hpp"
#include "State/sfDepthState.hpp"
#include "State/sfRasterState.hpp"
#include "State/sfSamplerState.hpp"

namespace Sulfur
{

GraphicsManager::GraphicsManager()
  : m_window(nullptr)
{
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::Init(Window& window)
{
  m_window = &window;
  InitDevice();

  g_SystemTable->DebugDraw->Init(m_device);
  m_renderWindow.Init(m_device, window);
  m_pipeline = new DeferredPipeline(m_device, &m_renderWindow);

  BlendState::InitDefaultStates(m_device);
  DepthState::InitDefaultStates(m_device);
  RasterState::InitDefaultStates(m_device);
  SamplerState::InitDefaultStates(m_device);
}

void GraphicsManager::Free()
{
  BlendState::FreeDefaultStates();
  DepthState::FreeDefaultStates();
  RasterState::FreeDefaultStates();
  SamplerState::FreeDefaultStates();

  m_device.Free();
}

void GraphicsManager::Update()
{
  m_pipeline->BuildCommandLists();
  m_pipeline->ExecuteCommandLists(m_device.GetImmediateContext());
  m_renderWindow.Present();
}

D3D11Device& GraphicsManager::GetDevice()
{
  return m_device;
}

void GraphicsManager::InitDevice()
{
  m_device.Init(D3D11Factory::Instance()->GetAdapters()[0]);
}

}
