/******************************************************************************/
/*!
\par     Sulfur
\file    sfRasterState.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Rasterizer state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRasterState.hpp"

namespace Sulfur
{

RasterState RasterState::s_states[StateCount];

void RasterState::Init(D3D11Device& device, const D3D11_RASTERIZER_DESC& description)
{
  ID3D11RasterizerState *state = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateRasterizerState(&description, &state),
    "Failed to create rasterizer state"
    );

  WrapperBase::Init(state);
}

void RasterState::Init(D3D11Device& device, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
  D3D11_RASTERIZER_DESC description;
  description.FillMode = fillMode;
  description.CullMode = cullMode;
  description.FrontCounterClockwise = FALSE;
  description.DepthBias = 0;
  description.DepthBiasClamp = 0.0f;
  description.SlopeScaledDepthBias = 0.0f;
  description.DepthClipEnable = TRUE;
  description.ScissorEnable = FALSE;
  description.MultisampleEnable = FALSE;
  description.AntialiasedLineEnable = FALSE;

  Init(device, description);
}

void RasterState::Free()
{
  WrapperBase::Free();
}

void RasterState::Set(D3D11Context& context)
{
  context.GetD3DResource()->RSSetState(m_resource);
}

void RasterState::Set(D3D11Context& context, States state)
{
  s_states[state].Set(context);
}

void RasterState::InitDefaultStates(D3D11Device& device)
{
  s_states[WIREFRAME].Init(device, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
  s_states[BACK_FACE_CULLING].Init(device, D3D11_FILL_SOLID, D3D11_CULL_BACK);
  s_states[FRONT_FACE_CULLING].Init(device, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
  s_states[NO_CULLING].Init(device, D3D11_FILL_SOLID, D3D11_CULL_NONE);
}

void RasterState::FreeDefaultStates()
{
  for (UINT32 i = 0; i < StateCount; ++i)
    s_states[i].Free();
}

}
