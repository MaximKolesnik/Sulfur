/******************************************************************************/
/*!
\par     Sulfur
\file    sfTexture2D.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   2D texture

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTexture2D.hpp"
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"

// Importers
#include "Modules/Resource/Importers/Texture/sfPngImporter.hpp"
#include "Modules/Resource/Importers/Texture/sfJpegImporter.hpp"
#include "Modules/Resource/Importers/Texture/sfDdsImporter.hpp"
#include <algorithm>

namespace Sulfur
{

SF_REGISTER_RESOURCE_TYPE(Texture2D)
  SF_IMPORTER(PngImporter, "png")
  SF_IMPORTER(JpegImporter, "jpg")
  SF_IMPORTER(DdsImporter<Texture2D>, "dds")
SF_END_REGISTER_RESOURCE_TYPE()

void Texture2D::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData)
{
  m_blurred = nullptr;
  m_blurredRenderTarget = nullptr;
  m_tempRenderTarget = nullptr;

  ID3D11Texture2D *texture = nullptr;
  D3D11_SUBRESOURCE_DATA *data = nullptr;

  if (pixelData != nullptr)
  {
    data = new D3D11_SUBRESOURCE_DATA[description.ArraySize * description.MipLevels];

    for (UINT32 i = 0; i < description.ArraySize; ++i)
    {
      UINT32 w = description.Width;
      UINT32 h = description.Height;

      for (UINT32 j = 0; j < description.MipLevels; ++j)
      {
        UINT32 rowPitch;
        UINT32 imgPitch;

        if (description.Format == DXGI_FORMAT_BC1_UNORM || description.Format == DXGI_FORMAT_BC2_UNORM || description.Format == DXGI_FORMAT_BC3_UNORM)
        {
          rowPitch = std::max(1U, (w + 3) / 4);
          if (description.Format == DXGI_FORMAT_BC1_UNORM)
            rowPitch <<= 3;
          else
            rowPitch <<= 4;
          imgPitch = rowPitch * std::max(1U, (h + 3) / 4);
        }
        else
        {
          rowPitch = (UINT32)(ceilf(GraphicsUtils::DxgiFormatBpp(description.Format) / 8.0f * w));
          imgPitch = rowPitch * h;
        }

        UINT32 index = i * description.MipLevels + j;
        data[index].pSysMem = pixelData;
        data[index].SysMemPitch = rowPitch;
        data[index].SysMemSlicePitch = imgPitch;

        w >>= 1;
        h >>= 1;
        pixelData += imgPitch;
      }
    }
  }

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateTexture2D(&description, data, &texture),
    "Failed to created 2D texture."
    );

  if (data)
    delete[] data;

  Init(texture);

  // Create shader resource
  if ((description.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
    CreateShaderResourceView(device);
}

void Texture2D::Init(const Texture2D& texture)
{
  Init(texture.GetD3DResource());
  m_shaderResourceView = texture.m_shaderResourceView;
}

void Texture2D::Free()
{
  if (m_blurred)
  {

    m_blurredRenderTarget->Free();
    delete m_blurred;
    delete m_blurredRenderTarget;
    m_blurred = nullptr;
    m_blurredRenderTarget = nullptr;
  }

  if (m_tempRenderTarget)
  {
    m_tempRenderTarget->Free();
    delete m_tempRenderTarget;
    m_tempRenderTarget = nullptr;
  }

  WrapperBase::Free();
}

void Texture2D::SetPixel(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->PSSetShaderResources(slot, 1, &m_shaderResourceView);
}

void Texture2D::GenerateMips(D3D11Context& context)
{
  context.GetD3DResource()->GenerateMips(m_shaderResourceView);
}

Texture2D* Texture2D::Blurred(D3D11Device& device, D3D11Context& context, DXGI_FORMAT format, bool cached, UINT32 downsamples, UINT32 iterations)
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

    m_blurred = new Texture2D();
    m_blurred->Init(device, description);
    m_blurredRenderTarget = new RenderTarget();
    m_blurredRenderTarget->Init(device, *m_blurred);
  }
  else
    description = m_blurred->GetDescription();

  SamplerState::SetPixel(context, SamplerState::LINEAR_CLAMP, 0);

  // Downsample
  if (downsamples > 0)
  {
    D3D11PixelShader *scaleShader = device.GetPixelShader("Shaders/PSScale.sbin");

    scaleShader->Set(context);
    m_blurredRenderTarget->Set(context);
    SetPixel(context, 0);
    GraphicsUtils::RenderFullscreenQuad(context);

    return m_blurred->Blurred(device, context, format, cached, downsamples - 1, iterations);
  }

  if (!m_tempRenderTarget)
  {
    m_tempRenderTarget = new RenderTarget();
    m_tempRenderTarget->Init(device, description);
  }

  Blur(device, context, this);
  for (UINT32 i = 1; i < iterations; ++i)
    Blur(device, context, m_blurred);

  context.ResetRenderTargets();
  return m_blurred;
}

void Texture2D::CreateShaderResourceView(D3D11Device& device)
{
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDescription;
  srvDescription.Format = m_description.Format;
  if (srvDescription.Format == DXGI_FORMAT_R24G8_TYPELESS) srvDescription.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
  if (srvDescription.Format == DXGI_FORMAT_R32G8X24_TYPELESS) srvDescription.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
  
  if ((m_description.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) != 0)
  {
    srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDescription.TextureCube.MostDetailedMip = 0;
    srvDescription.TextureCube.MipLevels = m_description.MipLevels;
  }
  else if (m_description.ArraySize == 1)
  {
    srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDescription.Texture2D.MostDetailedMip = 0;
    srvDescription.Texture2D.MipLevels = m_description.MipLevels;
  }
  else
  {
    srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDescription.Texture2DArray.MostDetailedMip = 0;
    srvDescription.Texture2DArray.MipLevels = m_description.MipLevels;
    srvDescription.Texture2DArray.FirstArraySlice = 0;
    srvDescription.Texture2DArray.ArraySize = m_description.ArraySize;
  }

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateShaderResourceView(m_resource, &srvDescription, &m_shaderResourceView),
    "Failed to created shader resource view."
    );
}

void Texture2D::Blur(D3D11Device& device, D3D11Context& context, Texture2D *source)
{
  D3D11PixelShader* vBlurShader = device.GetPixelShader("Shaders/PSGaussianVertical.sbin");
  D3D11PixelShader* hBlurShader = device.GetPixelShader("Shaders/PSGaussianHorizontal.sbin");

  vBlurShader->Set(context);
  m_tempRenderTarget->Set(context);
  source->SetPixel(context, 0);
  GraphicsUtils::RenderFullscreenQuad(context);
  context.ResetPixelTexture(0);

  hBlurShader->Set(context);
  m_blurredRenderTarget->Set(context);
  m_tempRenderTarget->GetTexture()->SetPixel(context, 0);
  GraphicsUtils::RenderFullscreenQuad(context);
  context.ResetPixelTexture(0);

}

}
