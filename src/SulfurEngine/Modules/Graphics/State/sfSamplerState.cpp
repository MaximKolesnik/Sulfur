/******************************************************************************/
/*!
\par     Sulfur
\file    sfSamplerState.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Sampler state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSamplerState.hpp"

namespace Sulfur
{

SamplerState SamplerState::s_states[StateCount];

void SamplerState::Init(D3D11Device& device, const D3D11_SAMPLER_DESC& description)
{
  ID3D11SamplerState *state = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateSamplerState(&description, &state),
    "Failed to create sampler state"
    );

  WrapperBase::Init(state);
}

void SamplerState::Free()
{
  WrapperBase::Free();
}

void SamplerState::SetVertex(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->VSSetSamplers(slot, 1, &m_resource);
}

void SamplerState::SetPixel(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->PSSetSamplers(slot, 1, &m_resource);
}

void SamplerState::SetVertex(D3D11Context& context, States state, UINT32 slot)
{
  s_states[state].SetVertex(context, slot);
}

void SamplerState::SetPixel(D3D11Context& context, States state, UINT32 slot)
{
  s_states[state].SetPixel(context, slot);
}

void SamplerState::InitDefaultStates(D3D11Device& device)
{
  D3D11_SAMPLER_DESC description;
  description.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  description.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  description.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  description.MipLODBias = 0.0f;
  description.MaxAnisotropy = 16;
  description.BorderColor[0] = 0.0f;
  description.BorderColor[1] = 0.0f;
  description.BorderColor[2] = 0.0f;
  description.BorderColor[3] = 0.0f;
  description.MinLOD = -FLT_MAX;
  description.MaxLOD = FLT_MAX;

  // Non-comparison states
  description.ComparisonFunc = D3D11_COMPARISON_NEVER;

  description.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  s_states[POINT].Init(device, description);

  description.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  s_states[LINEAR].Init(device, description);

  description.Filter = D3D11_FILTER_ANISOTROPIC;
  s_states[ANISOTROPIC].Init(device, description);

  // Comparison states
  description.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;

  description.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
  s_states[BILINEAR_COMPARISON].Init(device, description);

  description.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
  s_states[TRILINEAR_COMPARISON].Init(device, description);

  description.Filter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
  s_states[ANISOTROPIC_COMPARISON].Init(device, description);
}

void SamplerState::FreeDefaultStates()
{
  for (UINT32 i = 0; i < StateCount; ++i)
    s_states[i].Free();
}

}
