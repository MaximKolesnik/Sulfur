/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderNode.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Base render pipeline node

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRenderNode.hpp"

namespace Sulfur
{

RenderNode::RenderNode(D3D11Device& device)
{
  m_context.Init(device);
}

RenderNode::~RenderNode()
{
  m_context.Free();
}

D3D11Context& RenderNode::GetContext()
{
  return m_context;
}

}
