/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Context.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu 
\date    9/5/2016

\brief   

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfD3D11Context.hpp"
#include "sfD3D11Device.hpp"

namespace Sulfur
{

void D3D11Context::Init(D3D11Device& device, UINT32 flags)
{
  ID3D11DeviceContext *context = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateDeferredContext(flags, &context),
    "Failed to create deferred context."
    );

  Init(context);
}

void D3D11Context::ExecuteDeferredContext(D3D11Context& deferredContext)
{
  ID3D11CommandList *commandList = nullptr;

  SF_CRITICAL_ERR_HRESULT(
    deferredContext.m_resource->FinishCommandList(FALSE, &commandList),
    "Failed to create command list from deferred context"
    );

  m_resource->ExecuteCommandList(commandList, false);
  commandList->Release();
}

void D3D11Context::SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
  m_resource->IASetPrimitiveTopology(topology);
}

void D3D11Context::SetViewport(Real width, Real height)
{
  D3D11_VIEWPORT viewport = {
    0.0f, 0.0f,
    width, height,
    0.0f, 1.0f
  };
  m_resource->RSSetViewports(1, &viewport);
}

void D3D11Context::Draw(UINT32 vertexCount, UINT32 vertexStart)
{
  m_resource->Draw(vertexCount, vertexStart);
}

void D3D11Context::DrawIndexed(UINT32 indexCount, UINT32 indexStart, UINT32 vertexStart)
{
  m_resource->DrawIndexed(indexCount, indexStart, vertexStart);
}

}
