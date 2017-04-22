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
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Utils/sfGraphicsUtils.hpp"

// States
#include "State/sfBlendState.hpp"
#include "State/sfDepthState.hpp"
#include "State/sfRasterState.hpp"
#include "State/sfSamplerState.hpp"

namespace Sulfur
{

SF_DEFINE_TASK(ExecuteGraphicsPipelineNode)
{
  RenderNode *node = reinterpret_cast<RenderNode *>(_taskData->m_data);
  node->Process();
}
SF_END_DEFINE_TASK(ExecuteGraphicsPipelineNode)

SF_DEFINE_TASK(RenderGraphics)
{
  GraphicsManager *mgr = GraphicsManager::Instance();
  RenderPipeline *pipeline = mgr->GetPipeline();
  auto& nodeList = pipeline->GetNodes();
  RenderNode * const *nodes = nodeList.data();

  const UINT32 JOBS = 8;
  UINT32 jobIndex = 0;
  UINT32 nodeCount = (UINT32)nodeList.size();
  while (jobIndex < nodeCount)
  {
    UINT32 jobsLeft = nodeCount - jobIndex;
    UINT32 jobsToRun = std::min(jobsLeft, JOBS);
    SF_ENQUEUE_JOBS(ExecuteGraphicsPipelineNode, ((void**)(nodes + jobIndex)), JOBS, jobsToRun);
    SF_YEILD_AND_WAIT();
    jobIndex += jobsToRun;
  }

  mgr->FinishRender();
}
SF_END_DEFINE_TASK(RenderGraphics)

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

  DebugDraw::Instance()->Init(m_device);
  m_renderWindow.Init(m_device, window);
  m_pipeline = new DeferredPipeline(m_device, &m_renderWindow);

  BlendState::InitDefaultStates(m_device);
  DepthState::InitDefaultStates(m_device);
  RasterState::InitDefaultStates(m_device);
  SamplerState::InitDefaultStates(m_device);

  GraphicsUtils::Init(m_device);
}

void GraphicsManager::Free()
{
  BlendState::FreeDefaultStates();
  DepthState::FreeDefaultStates();
  RasterState::FreeDefaultStates();
  SamplerState::FreeDefaultStates();

  GraphicsUtils::Free();

  m_device.Free();
}

void GraphicsManager::FinishRender()
{
  m_pipeline->ExecuteCommandLists(m_device.GetImmediateContext());
  m_renderWindow.Present();
}

D3D11Device& GraphicsManager::GetDevice()
{
  return m_device;
}

RenderPipeline* GraphicsManager::GetPipeline()
{
  return m_pipeline;
}

void GraphicsManager::InitDevice()
{
  m_device.Init(D3D11Factory::Instance()->GetAdapters()[0]);
}

}
