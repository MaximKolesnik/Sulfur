/******************************************************************************/
/*!
\par     Sulfur
\file    sfSamplerState.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Sampler state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class SamplerState : public D3D11ResourceWrapper<ID3D11SamplerState, D3D11_SAMPLER_DESC>
  {

  public:
    enum States
    {
      POINT,
      LINEAR,
      ANISOTROPIC,
      BILINEAR_COMPARISON,
      TRILINEAR_COMPARISON,
      ANISOTROPIC_COMPARISON,
      StateCount
    };

  public:
    void Init(D3D11Device& device, const D3D11_SAMPLER_DESC& description);
    virtual void Free() override;

    void SetVertex(D3D11Context& context, UINT32 slot);
    void SetPixel(D3D11Context& context, UINT32 slot);
  
    static void SetVertex(D3D11Context& context, States state, UINT32 slot);
    static void SetPixel(D3D11Context& context, States state, UINT32 slot);

    static void InitDefaultStates(D3D11Device& device);
    static void FreeDefaultStates();

  private:
    static SamplerState s_states[StateCount];

  };
  
}