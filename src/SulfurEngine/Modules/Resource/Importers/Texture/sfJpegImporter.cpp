/******************************************************************************/
/*!
\par     Sulfur
\file    sfJpegImporter.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/3/2016

\brief   JPEG texture importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfJpegImporter.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include <jpeglib.h>
#include <stdio.h>

namespace Sulfur
{

void JpegImporter::Load(BYTE *buffer, UINT32 bufferSize, Texture2D *resource)
{
  jpeg_decompress_struct decompress;
  jpeg_error_mgr error;

  decompress.err = jpeg_std_error(&error);
  jpeg_create_decompress(&decompress);
  jpeg_mem_src(&decompress, buffer, bufferSize);

  jpeg_read_header(&decompress, TRUE);
  jpeg_start_decompress(&decompress);

  UINT32 pixelCount = decompress.output_width * decompress.output_height;
  BYTE *pixelData = new BYTE[pixelCount * 3];
  while (decompress.output_scanline < decompress.output_height)
  {
    BYTE *rowPtr = pixelData + 3 * decompress.output_width * decompress.output_scanline;
    jpeg_read_scanlines(&decompress, &rowPtr, 1);
  }

  BYTE *rgba = new BYTE[pixelCount * 4];
  for (UINT32 i = 0; i < pixelCount; ++i)
  {
    rgba[i * 4 + 0] = pixelData[i * 3 + 0];
    rgba[i * 4 + 1] = pixelData[i * 3 + 1];
    rgba[i * 4 + 2] = pixelData[i * 3 + 2];
    rgba[i * 4 + 3] = 255U;
  }

  D3D11_TEXTURE2D_DESC description;
  description.Width = decompress.output_width;
  description.Height = decompress.output_height;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;
  resource->Init(GraphicsManager::Instance()->GetDevice(), description, rgba);

  jpeg_finish_decompress(&decompress);
  delete[] pixelData;
  delete[] rgba;
}

}
