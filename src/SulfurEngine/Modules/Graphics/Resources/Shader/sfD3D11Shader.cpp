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
#include "sfD3D11Shader.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Utils/sfFileUtils.hpp"

namespace Sulfur
{

void D3D11Shader::Init(D3D11Device& device, const std::string& filePath)
{
  BYTE *buffer = FileUtils::ReadFile(filePath);
  UINT32 fileSize = FileUtils::GetFileSize(filePath);

  Init(device, buffer, fileSize);

  delete[] buffer;
}

void D3D11Shader::Init(D3D11Device& device, const BYTE *compiledShader, UINT32 size)
{
  ReflectShader(compiledShader, size);
  CreateConstantBuffers(device);
  CreateInputLayout(device, compiledShader, size);
}

void D3D11Shader::Free()
{

}

D3D11ConstantBuffer* D3D11Shader::GetConstantBuffer(const std::string& name)
{
  auto it = m_cbufferMap.find(name);
  if (it == m_cbufferMap.end())
    return nullptr;

  return m_cbuffers.data() + it->second;
}

void D3D11Shader::ReflectShader(const BYTE *compiledShader, UINT32 size)
{
  D3DReflect(compiledShader, size, IID_ID3D11ShaderReflection, (void**)&m_reflector);
  m_reflector->GetDesc(&m_description);
}

void D3D11Shader::CreateConstantBuffers(D3D11Device& device)
{
  for (UINT32 i = 0; i < m_description.ConstantBuffers; ++i)
  {
    ID3D11ShaderReflectionConstantBuffer *cbufferReflector = m_reflector->GetConstantBufferByIndex(i);

    D3D11_SHADER_BUFFER_DESC cbufferDescription;
    cbufferReflector->GetDesc(&cbufferDescription);

    D3D11ConstantBuffer cbuffer;
    cbuffer.Init(device, cbufferDescription.Size);

    m_cbufferMap[cbufferDescription.Name] = (UINT32)m_cbuffers.size();
    m_cbuffers.push_back(cbuffer);
  }
}

void D3D11Shader::CreateInputLayout(D3D11Device& device, const BYTE *compiledShader, UINT32 size)
{
  if (m_description.InputParameters == 0)
  {
    m_inputLayout = nullptr;
    return;
  }

  D3D11_INPUT_ELEMENT_DESC *inputElements = new D3D11_INPUT_ELEMENT_DESC[m_description.InputParameters];
  D3D11_SIGNATURE_PARAMETER_DESC parameterDescription;
  for (UINT32 i = 0; i < m_description.InputParameters; ++i)
  {
    // TODO: Add instancing support
    m_reflector->GetInputParameterDesc(i, &parameterDescription);
    inputElements[i].SemanticName = parameterDescription.SemanticName;
    inputElements[i].SemanticIndex = parameterDescription.SemanticIndex;
    inputElements[i].Format = GraphicsUtils::DxgiFormatFromTypeAndMask(parameterDescription.ComponentType, parameterDescription.Mask);
    inputElements[i].InputSlot = 0;
    inputElements[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputElements[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElements[i].InstanceDataStepRate = 0;
  }

  device.GetD3DResource()->CreateInputLayout(inputElements, m_description.InputParameters, compiledShader, size, &m_inputLayout);
  delete[] inputElements;
}

}
