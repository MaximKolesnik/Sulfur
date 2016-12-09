/******************************************************************************/
/*!
\par     Sulfur
\file    sfBlendState.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Blend state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"
#include "Math/sfVector4.hpp"

namespace Sulfur
{
	
  class BlendState : public D3D11ResourceWrapper<ID3D11BlendState, D3D11_BLEND_DESC>
  {

  public:
    enum States
    {
      NO_BLENDING,
      ALPHA,
      ADDITIVE,
      MULTIPLY,
      StateCount
    };

  public:
    void Init(D3D11Device& device, const D3D11_BLEND_DESC& description, const Vector4& blendFactor = Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    virtual void Free() override;

    void Set(D3D11Context& context);
    static void Set(D3D11Context& context, States state);

    static void InitDefaultStates(D3D11Device& device);
    static void FreeDefaultStates();

  private:
    Vector4 m_blendFactor;
    
    static BlendState s_states[StateCount];
  
  };
  
}