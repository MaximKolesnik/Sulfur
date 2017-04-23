/******************************************************************************/
/*!
\par     Sulfur
\file    sfRasterState.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   Rasterizer state wrapper

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class RasterState : public D3D11ResourceWrapper<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>
  {

  public:
    enum States
    {
      WIREFRAME,
      BACK_FACE_CULLING,
      FRONT_FACE_CULLING,
      NO_CULLING,
      StateCount
    };

  public:
    void Init(D3D11Device& device, const D3D11_RASTERIZER_DESC& description);
    void Init(D3D11Device& device, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
    virtual void Free() override;

    void Set(D3D11Context& context);
    static void Set(D3D11Context& context, States state);

    static void InitDefaultStates(D3D11Device& device);
    static void FreeDefaultStates();

  private:
    static RasterState s_states[StateCount];
  
  };
  
}