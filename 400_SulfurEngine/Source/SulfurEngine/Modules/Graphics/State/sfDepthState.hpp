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
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class DepthState : public D3D11ResourceWrapper<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>
  {

  public:
    enum States
    {
      DEPTH_DISABLED,
      DEPTH_ENABLED,
      StateCount
    };

  public:
    void Init(D3D11Device& device, const D3D11_DEPTH_STENCIL_DESC& description);
    void Init(D3D11Device& device, bool depthEnabled, bool stencilEnabled);
    virtual void Free() override;

    void Set(D3D11Context& context);
    static void Set(D3D11Context& context, States state);

    static void InitDefaultStates(D3D11Device& device);
    static void FreeDefaultStates();

  private:
    static DepthState s_states[StateCount];
  
  };
  
}