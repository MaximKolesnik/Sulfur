/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Context.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 device context

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Wrapper.hpp"

namespace Sulfur
{

  class D3D11Device;
	
  class D3D11Context : public D3D11ResourceWrapper<ID3D11DeviceContext>
  {
  
  public:
    using WrapperBase::Init;
    void Init(D3D11Device& device, UINT32 flags = 0);

    void ExecuteDeferredContext(D3D11Context& deferredContext);

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

    void SetViewport(Real width, Real height);

    void Draw(UINT32 vertexCount, UINT32 vertexStart);
    void DrawIndexed(UINT32 indexCount, UINT32 indexStart, UINT32 vertexStart);

  };
  
}