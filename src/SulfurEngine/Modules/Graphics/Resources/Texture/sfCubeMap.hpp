/******************************************************************************/
/*!
\par     Sulfur
\file    sfCubeMap.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   Cubemap texture

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfTexture2D.hpp"

namespace Sulfur
{
	
  class CubeMap : public Texture2D
  {

  public:
    using Texture2D::Init;
    void Init(ID3D11Texture2D *texture);
    void Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData = nullptr);

    CubeMap* Convolved(D3D11Device& device);

  protected:
    virtual void CreateShaderResourceView(D3D11Device& device) override;

  private:
    CubeMap *m_convolved;

  };
  
}