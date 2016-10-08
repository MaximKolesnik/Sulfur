/******************************************************************************/
/*!
\par     Sulfur
\file    sfBlendState.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Blend state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfBlendState.hpp"

namespace Sulfur
{

BlendState BlendState::s_states[StateCount];

void BlendState::Init(D3D11Device& device, const D3D11_BLEND_DESC& description, const Vector4& blendFactor)
{
  m_blendFactor = blendFactor;
  ID3D11BlendState *state = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateBlendState(&description, &state),
    "Failed to create blend state"
    );

  WrapperBase::Init(state);
}

void BlendState::Free()
{
  WrapperBase::Free();
}

void BlendState::Set(D3D11Context& context)
{
  context.GetD3DResource()->OMSetBlendState(m_resource, &m_blendFactor[0], 0xFFFFFFFF);
}

void BlendState::Set(D3D11Context& context, States state)
{
  s_states[state].Set(context);
}

void BlendState::InitDefaultStates(D3D11Device& device)
{
  D3D11_BLEND_DESC description;

  // No Blend
  description.AlphaToCoverageEnable = FALSE;
  description.IndependentBlendEnable = FALSE;
  description.RenderTarget[0].BlendEnable = FALSE;
  description.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  s_states[NO_BLENDING].Init(device, description);

  // Alpha blending
  description.RenderTarget[0].BlendEnable = TRUE;
  description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  s_states[ALPHA].Init(device, description);

  // Additive blending
  description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  description.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
  description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
  description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  s_states[ADDITIVE].Init(device, description);

  // Multiply blending
  description.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
  description.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
  description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
  description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
  s_states[MULTIPLY].Init(device, description);
}

void BlendState::FreeDefaultStates()
{
  for (UINT32 i = 0; i < StateCount; ++i)
    s_states[i].Free();
}

}
