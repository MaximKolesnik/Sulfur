/******************************************************************************/
/*!
\par     Sulfur
\file    sfClass.cpp
\author  
\par     DP email: 
\date    9/5/2016

\brief   

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfMeshRenderer.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

void MeshRenderer::Initialize(void)
{

}

MeshRenderer* MeshRenderer::Clone(void) const
{
  MeshRenderer *newMeshRenderer = SF_CREATE_COMP(MeshRenderer);
  return newMeshRenderer;
}

Mesh* MeshRenderer::GetMesh()
{
  return m_mesh;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
  m_mesh = mesh;
}

}