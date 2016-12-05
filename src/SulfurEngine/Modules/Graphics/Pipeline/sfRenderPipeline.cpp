/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderPipeline.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Render pipeline

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderPipeline.hpp"

namespace Sulfur
{

void RenderPipeline::BuildCommandLists()
{
  for (RenderNode *node : m_nodes)
    node->Process();
}

void RenderPipeline::ExecuteCommandLists(D3D11Context& immediateContext)
{
  for (RenderNode *node : m_nodes)
    immediateContext.ExecuteDeferredContext(node->GetContext());
}

void RenderPipeline::AddNode(RenderNode *node)
{
  m_nodes.push_back(node);
}

const RenderPipeline::NodeList& RenderPipeline::GetNodes() const
{
  return m_nodes;
}

}
