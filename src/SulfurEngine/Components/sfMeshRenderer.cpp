/******************************************************************************/
/*!
\par     Sulfur
\file    sfClass.cpp
\author  
\par     DP email: 
\date    9/5/2016

\brief   

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfMeshRenderer.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

namespace Sulfur
{

  MeshRenderer::MeshRenderer()
    : m_meshResource(nullptr), m_meshResourcePath(), m_material()
  {
  }


  void MeshRenderer::Initialize(void)
  {

  }

  MeshRenderer* MeshRenderer::Clone(void) const
  {
    MeshRenderer *newMeshRenderer = SF_CREATE_COMP(MeshRenderer);
    return newMeshRenderer;
  }

  void MeshRenderer::DrawDebug(DebugDraw *draw) const
  {
    draw->DrawWireframe(this);
  }

}
