/******************************************************************************/
/*!
\par     Sulfur
\file    sfDepthState.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/2/2016

\brief   Depth stencil state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDepthState.hpp"

namespace Sulfur
{

DepthState DepthState::s_states[StateCount];

void DepthState::Init(D3D11Device& device, const D3D11_DEPTH_STENCIL_DESC& description)
{
  ID3D11DepthStencilState *state = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateDepthStencilState(&description, &state),
    "Failed to create depth stencil state"
    );

  WrapperBase::Init(state);
}

void DepthState::Init(D3D11Device& device, bool depthEnabled, bool stencilEnabled)
{
  D3D11_DEPTH_STENCIL_DESC description;
  description.DepthEnable = depthEnabled;
  description.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  description.DepthFunc = D3D11_COMPARISON_LESS;
  description.StencilEnable = stencilEnabled;
  description.StencilReadMask = 0xFF;
  description.StencilWriteMask = 0xFF;

  // Front-face operations
  description.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  description.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  description.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  description.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Back-face operations
  description.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  description.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  description.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  description.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  Init(device, description);
}

void DepthState::Free()
{
  WrapperBase::Free();
}

void DepthState::Set(D3D11Context& context)
{
  context.GetD3DResource()->OMSetDepthStencilState(m_resource, 1);
}

void DepthState::Set(D3D11Context& context, States state)
{
  s_states[state].Set(context);
}

void DepthState::InitDefaultStates(D3D11Device& device)
{
  s_states[DEPTH_DISABLED].Init(device, false, false);
  s_states[DEPTH_ENABLED].Init(device, true, true);
}

void DepthState::FreeDefaultStates()
{
  for (UINT32 i = 0; i < StateCount; ++i)
    s_states[i].Free();
}

}
