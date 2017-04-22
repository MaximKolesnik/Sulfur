/******************************************************************************/
/*!
\par     Sulfur
\file    sfDdsImporter.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/4/2016

\brief   DDS texture importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Resource/sfResourceImporter.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include <dds.h>

namespace Sulfur
{
	
  void DdsHeaderToTextureDescription(const DirectX::DDS_HEADER *header, D3D11_TEXTURE2D_DESC& description);

  template <typename TextureType>
  class DdsImporter : public IResourceImporter<TextureType>
  {
  
  public:
    virtual void Load(BYTE *buffer, UINT32 bufferSize, TextureType *resource) override
    {
      uint32_t *magic = reinterpret_cast<uint32_t*>(buffer);

      SF_CRITICAL_ERR_EXP(
        *magic == DirectX::DDS_MAGIC,
        "Invalid DDS file"
        );

      DirectX::DDS_HEADER *header = reinterpret_cast<DirectX::DDS_HEADER*>(buffer + sizeof(uint32_t));
      BYTE *pixelData = buffer + sizeof(uint32_t) + header->dwSize;

      D3D11_TEXTURE2D_DESC description;
      DdsHeaderToTextureDescription(header, description);

      resource->Init(GraphicsManager::Instance()->GetDevice(), description, pixelData);
    }

  };
  
}