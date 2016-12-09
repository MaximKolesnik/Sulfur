/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Device.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 device

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Wrapper.hpp"
#include "sfD3D11Context.hpp"
#include "sfD3D11Adapter.hpp"

namespace Sulfur
{

class D3D11VertexShader;
class D3D11PixelShader;

class D3D11Device : public D3D11ResourceWrapper<ID3D11Device>
{
  
public:
  void Init(D3D11Adapter& adapter);
  void Free();

  // Contexts
  D3D11Context& GetImmediateContext();
  const D3D11Context& GetImmediateContext() const;

  // Shaders
  D3D11VertexShader* GetVertexShader(const std::string& file);
  D3D11PixelShader* GetPixelShader(const std::string& file);

private:
  D3D_FEATURE_LEVEL m_featureLevel;
  D3D11Context m_immediateContext;

  std::unordered_map<std::string, D3D11VertexShader*> m_vertexShaders;
  std::unordered_map<std::string, D3D11PixelShader*> m_pixelShaders;

};
  
}