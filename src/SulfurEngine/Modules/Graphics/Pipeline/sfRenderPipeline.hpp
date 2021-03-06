/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderPipeline.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Render pipeline

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfRenderNode.hpp"

namespace Sulfur
{
	
  class RenderPipeline
  {
  
  public:
    typedef std::vector<RenderNode*> NodeList;

  public:
    void BuildCommandLists();
    void ExecuteCommandLists(D3D11Context& immediateContext);

    void AddNode(RenderNode *node);
    const NodeList& GetNodes() const;

  private:
    NodeList m_nodes;

  };
  
}