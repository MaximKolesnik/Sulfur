/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11ComputeShader.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Direct3D 11 Compute Shader

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11ComputeShader.hpp"

namespace Sulfur
{

void D3D11ComputeShader::Init(D3D11Device& device, const BYTE *compiledShader, UINT32 size)
{
  device.GetD3DResource()->CreateComputeShader(compiledShader, size, nullptr, &m_shader);
  D3D11Shader::Init(device, compiledShader, size);
}

void D3D11ComputeShader::Free()
{
  m_shader->Release();
}

void D3D11ComputeShader::Set(D3D11Context& context)
{
  context.GetD3DResource()->CSSetShader(m_shader, nullptr, 0);

  for (UINT32 i = 0; i < m_cbuffers.size(); ++i)
    m_cbuffers[i].SetCompute(context, i);
}

}
