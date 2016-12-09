/******************************************************************************/
/*!
\par     Sulfur
\file    sfDdsImporter.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   DDS texture importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDdsImporter.hpp"

namespace Sulfur
{

void DdsHeaderToTextureDescription(const DirectX::DDS_HEADER *header, D3D11_TEXTURE2D_DESC& description)
{
  bool isCubemap = (header->dwCaps2 & DDS_CUBEMAP) != 0;

  description.Width = header->dwWidth;
  description.Height = header->dwHeight;
  description.MipLevels = (header->dwFlags & DDS_HEADER_FLAGS_MIPMAP) != 0 ? header->dwMipMapCount : 1;
  description.ArraySize = isCubemap ? 6 : 1;
  description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = isCubemap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;

  if (memcmp(&header->ddspf, &DirectX::DDSPF_DXT1, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_BC1_UNORM;
  else if(memcmp(&header->ddspf, &DirectX::DDSPF_DXT3, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_BC2_UNORM;
  else if (memcmp(&header->ddspf, &DirectX::DDSPF_DXT5, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_BC3_UNORM;
  else if (memcmp(&header->ddspf, &DirectX::DDSPF_A8B8G8R8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  else if (memcmp(&header->ddspf, &DirectX::DDSPF_A8R8G8B8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  else if (memcmp(&header->ddspf, &DirectX::DDSPF_X8R8G8B8, sizeof(DirectX::DDS_PIXELFORMAT)) == 0)
    description.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
  else
    SF_CRITICAL_ERR("Unsupported DDS pixel format");
}

}
