/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Shader.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 base shader

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"

namespace Sulfur
{
	
  class D3D11Shader
  {

  public:
    void Init(D3D11Device& device, const std::string& filePath);
    virtual void Init(D3D11Device& device, const BYTE *compiledShader, UINT32 size);
    virtual void Free();

    D3D11ConstantBuffer* GetConstantBuffer(const std::string& name);

  private:
    void ReflectShader(const BYTE *compiledShader, UINT32 size);
    void CreateConstantBuffers(D3D11Device& device);
    void CreateInputLayout(D3D11Device& device, const BYTE *compiledShader, UINT32 size);
  
  protected:
    // Reflection
    D3D11_SHADER_DESC m_description;
    ID3D11ShaderReflection *m_reflector;

    // Constant buffers
    std::vector<D3D11ConstantBuffer> m_cbuffers;
    std::unordered_map<std::string, UINT32> m_cbufferMap;

    ID3D11InputLayout *m_inputLayout;

  };
  
}