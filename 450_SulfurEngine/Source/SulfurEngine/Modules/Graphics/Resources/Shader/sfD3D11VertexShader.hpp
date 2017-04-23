/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11VertexShader.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Direct3D 11 Vertex Shader

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Shader.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class D3D11VertexShader : public D3D11Shader
  {

  public:
    using D3D11Shader::Init;
    virtual void Init(D3D11Device& device, const BYTE *compiledShader, UINT32 size) override;
    virtual void Free() override;

    void Set(D3D11Context& context);

  private:
    ID3D11VertexShader *m_shader;
  
  };
  
}