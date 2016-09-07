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
#include "Utils/sfFileUtils.hpp"
#include <png.h>

namespace Sulfur
{

void Texture2D::Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description, const BYTE *pixelData)
{
  ID3D11Texture2D *texture = nullptr;

  // TODO: Add mip support
  D3D11_SUBRESOURCE_DATA *data = nullptr;

  if (pixelData != nullptr)
  {
    UINT32 rowPitch = (UINT32)(GraphicsUtils::DxgiFormatBpp(description.Format) / 8.0f * description.Width);
    UINT32 imgPitch = rowPitch * description.Height;

    data = new D3D11_SUBRESOURCE_DATA[description.ArraySize];

    for (UINT32 i = 0; i < description.ArraySize; ++i)
    {
      data[i].pSysMem = pixelData + imgPitch * i;
      data[i].SysMemPitch = rowPitch;
      data[i].SysMemSlicePitch = 0;
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

void Texture2D::Init(D3D11Device& device, const std::string& fileName)
{
  BYTE *pngImageData = FileUtils::ReadFile(fileName);
  UINT32 size = FileUtils::GetFileSize(fileName);
  png_image image = { 0 };
  image.version = PNG_IMAGE_VERSION;

  if (png_image_begin_read_from_memory(&image, pngImageData, size))
  {
    image.format = PNG_FORMAT_RGBA;
    BYTE *pixelData = new BYTE[PNG_IMAGE_SIZE(image)];

    if (png_image_finish_read(&image, nullptr, pixelData, image.width * 4, nullptr))
    {
      D3D11_TEXTURE2D_DESC description;
      description.Width = image.width;
      description.Height = image.height;
      description.MipLevels = 1;
      description.ArraySize = 1;
      description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      description.SampleDesc.Count = 1;
      description.SampleDesc.Quality = 0;
      description.Usage = D3D11_USAGE_DEFAULT;
      description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      description.CPUAccessFlags = 0;
      description.MiscFlags = 0;

      Init(device, description, pixelData);
    }

    delete[] pixelData;
    png_image_free(&image);
  }

  delete[] pngImageData;
}

void Texture2D::SetPixel(D3D11Context& context, UINT32 slot)
{
  context.GetD3DResource()->PSSetShaderResources(slot, 1, &m_shaderResourceView);
}

void Texture2D::CreateShaderResourceView(D3D11Device& device)
{
  // TODO: Add texture array support
  D3D11_SHADER_RESOURCE_VIEW_DESC srvDescription;
  srvDescription.Format = m_description.Format;
  srvDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvDescription.Texture2D.MipLevels = m_description.MipLevels;
  srvDescription.Texture2D.MostDetailedMip = 0;

  SF_CRITICAL_ERR_HRESULT(
    device.GetD3DResource()->CreateShaderResourceView(m_resource, &srvDescription, &m_shaderResourceView),
    "Failed to created shader resource view."
    );
}

}
