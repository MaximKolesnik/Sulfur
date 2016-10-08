/******************************************************************************/
/*!
\par     Sulfur
\file    sfPngImporter.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/24/2016

\brief   PNG texture importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfPngImporter.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include <png.h>

namespace Sulfur
{

void PngImporter::Load(BYTE *buffer, UINT32 bufferSize, Texture2D *resource)
{
  png_image image = { 0 };
  image.version = PNG_IMAGE_VERSION;

  if (png_image_begin_read_from_memory(&image, buffer, bufferSize))
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

      resource->Init(g_SystemTable->GraphicsManager->GetDevice(), description, pixelData);
    }

    delete[] pixelData;
    png_image_free(&image);
  }
}

}
