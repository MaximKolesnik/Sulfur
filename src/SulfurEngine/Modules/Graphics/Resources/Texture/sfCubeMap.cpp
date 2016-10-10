/******************************************************************************/
/*!
\par     Sulfur
\file    sfCubeMap.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   Cubemap texture

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfCubeMap.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"

// Importers
#include "Modules/Resource/Importers/Texture/sfDdsImporter.hpp"

namespace Sulfur
{

SF_REGISTER_RESOURCE_TYPE(CubeMap)
  SF_IMPORTER(DdsImporter<CubeMap>, "dds")
SF_END_REGISTER_RESOURCE_TYPE()

void CubeMap::Init(ID3D11Texture2D *texture)
{
  D3D11_TEXTURE2D_DESC description;
  texture->GetDesc(&description);

  SF_CRITICAL_ERR_EXP(
    description.ArraySize % 6 == 0,
    "Cubemaps' array sizes must be a multiple of 6"
    );

  WrapperBase::Init(texture);
  m_convolved = nullptr;
}

void CubeMap::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData)
{
  SF_CRITICAL_ERR_EXP(
    description.ArraySize % 6 == 0,
    "Cubemaps' array sizes must be a multiple of 6"
    );

  Texture2D::Init(device, description, pixelData);
  m_convolved = nullptr;
}

CubeMap* CubeMap::Convolved(D3D11Device& device)
{
  if (m_convolved != nullptr)
    return m_convolved;

  D3D11_TEXTURE2D_DESC description = GetDescription();
  description.Width = description.Height = 128;
  description.MipLevels = 1;
  description.BindFlags |= D3D11_BIND_RENDER_TARGET;
  description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

  D3D11VertexShader vertexShader;
  vertexShader.Init(device, "Shaders/VSFullScreenQuad.sbin");
  vertexShader.Set(device.GetImmediateContext());

  D3D11PixelShader pixelShader;
  pixelShader.Init(device, "Shaders/PSCubemapConvolution.sbin");
  pixelShader.Set(device.GetImmediateContext());

  D3D11ConstantBuffer *convolutionDataBuffer = pixelShader.GetConstantBuffer("ConvolutionData");
  ConvolutionData convolutionData;
  convolutionData.OutputWidth = description.Width;
  convolutionData.OutputHeight = description.Height;
  convolutionData.PhiRange = 6.283f;
  convolutionData.PhiStep = 0.012f;
  convolutionData.ThetaRange = 1.57f;
  convolutionData.ThetaStep = 0.05f;

  device.GetImmediateContext().SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

  RenderTarget target;
  target.Init(device, description);
  target.Clear(device.GetImmediateContext(), Vector4(0.0f, 0.0f, 0.0f, 0.0f));

  SetPixel(device.GetImmediateContext(), 0);

  SamplerState::SetPixel(device.GetImmediateContext(), SamplerState::LINEAR, 0);

  for (UINT32 i = 0; i < 6; ++i)
  {
    target.Set(device.GetImmediateContext(), i, 1);

    convolutionData.CubeFace = i;
    convolutionDataBuffer->SetData(device.GetImmediateContext(), convolutionData);

    device.GetImmediateContext().Draw(6, 0);
  }

  m_convolved = new CubeMap();
  m_convolved->Init(*target.GetTexture());

  target.RenderTarget::WrapperBase::Free();
  vertexShader.Free();
  pixelShader.Free();

  return m_convolved;
}

void CubeMap::CreateShaderResourceView(D3D11Device& device)
{
  // TODO: Add cubemap array support
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDescription;
  srvDescription.Format = m_description.Format;
  srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
  srvDescription.TextureCube.MipLevels = m_description.MipLevels;
  srvDescription.TextureCube.MostDetailedMip = 0;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateShaderResourceView(m_resource, &srvDescription, &m_shaderResourceView),
    "Failed to created shader resource view."
    );
}

}
