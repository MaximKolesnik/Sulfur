/******************************************************************************/
/*!
\par     Sulfur
\file    sfTexture2D.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   2D texture

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class Texture2D : public D3D11ResourceWrapper<ID3D11Texture2D, D3D11_TEXTURE2D_DESC>
  {

  public:
    using WrapperBase::Init;
    void Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData = nullptr);
    void Init(const Texture2D& texture);

    void SetPixel(D3D11Context& context, UINT32 slot);

  protected:
    virtual void CreateShaderResourceView(D3D11Device& device);

  protected:
    ID3D11ShaderResourceView *m_shaderResourceView;

  };
  
}