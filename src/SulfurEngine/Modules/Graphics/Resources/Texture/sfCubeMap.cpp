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
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"

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
  m_blurred = nullptr;
  m_blurredRenderTarget = nullptr;
}

void CubeMap::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData)
{
  SF_CRITICAL_ERR_EXP(
    description.ArraySize % 6 == 0,
    "Cubemaps' array sizes must be a multiple of 6"
    );

  Texture2D::Init(device, description, pixelData);
}

Texture2D* CubeMap::Blurred(D3D11Device& device, D3D11Context& context, DXGI_FORMAT format, bool cached, UINT32 downsamples, UINT32 iterations)
{
  D3D11_TEXTURE2D_DESC description;

  if (m_blurred != nullptr && cached)
  {
    if (downsamples > 0)
      return m_blurred->Blurred(device, context, format, cached, downsamples - 1, iterations);
    return m_blurred;
  }
  else if (!m_blurred)
  {
    description = GetDescription();
    description.Width /= 2;
    description.Height /= 2;
    description.MipLevels = 1;
    description.BindFlags |= D3D11_BIND_RENDER_TARGET;
    description.Format = format;

    m_blurred = new CubeMap();
    m_blurred->Init(device, description);
    m_blurredRenderTarget = new RenderTarget();
    m_blurredRenderTarget->Init(device, *m_blurred);
  }
  else
    description = m_blurred->GetDescription();

  SamplerState::SetPixel(context, SamplerState::LINEAR, 0);
  SetPixel(context, 0);

  // Downsample
  if (downsamples > 0)
  {
    D3D11PixelShader *cubeMapScaleShader = device.GetPixelShader("Shaders/PSScaleCubemap.sbin");
    D3D11ConstantBuffer *cubeMapScaleDataBuffer = cubeMapScaleShader->GetConstantBuffer("CubemapScaleData");
    CubemapScaleData cubeMapScaleData;

    cubeMapScaleShader->Set(context);
    for (UINT32 i = 0; i < 6; ++i)
    {
      m_blurredRenderTarget->Set(context, i, 1);

      cubeMapScaleData.CubeFace = i;
      cubeMapScaleDataBuffer->SetData(context, cubeMapScaleData);

      GraphicsUtils::RenderFullscreenQuad(context, 3.14159f / 4.0f, 1.0f, 1.0f);
    }

    context.ResetRenderTargets();
    return m_blurred->Blurred(device, context, format, cached, downsamples - 1, iterations);
  }

  D3D11PixelShader* convolutionShader = device.GetPixelShader("Shaders/PSCubemapConvolution.sbin");
  D3D11ConstantBuffer *convolutionDataBuffer = convolutionShader->GetConstantBuffer("ConvolutionData");
  ConvolutionData convolutionData;
  convolutionData.OutputWidth = description.Width;
  convolutionData.OutputHeight = description.Height;
  convolutionData.PhiRange = 6.283f;
  convolutionData.PhiStep = 0.036f;
  convolutionData.ThetaRange = 1.57f;
  convolutionData.ThetaStep = 0.15f;

  convolutionShader->Set(context);
  m_blurredRenderTarget->Clear(context, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  for (UINT32 i = 0; i < 6; ++i)
  {
    m_blurredRenderTarget->Set(context, i, 1);

    convolutionData.CubeFace = i;
    convolutionDataBuffer->SetData(context, convolutionData);

    GraphicsUtils::RenderFullscreenQuad(context, 3.14159f / 4.0f, 1.0f, 1.0f);
  }

  context.ResetRenderTargets();
  return m_blurred;
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
