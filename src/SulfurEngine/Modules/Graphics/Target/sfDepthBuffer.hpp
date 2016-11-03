/******************************************************************************/
/*!
\par     Sulfur
\file    sfDepthBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Depth buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"

namespace Sulfur
{
	
  class DepthBuffer : public D3D11ResourceWrapper<ID3D11DepthStencilView, D3D11_DEPTH_STENCIL_VIEW_DESC>
  {

  public:
    void Init(D3D11Device& device, Texture2D& texture);
    void Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description);
    virtual void Free() override;

    void Resize(UINT32 width, UINT32 height);

    void Set(D3D11Context& context, UINT32 index = 0);
    void Clear(D3D11Context& context);

  private:
    D3D11Device *m_device;
    Texture2D m_texture;
  
  };
  
}