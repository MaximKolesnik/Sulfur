/******************************************************************************/
/*!
\par     Sulfur
\file    sfPngImporter.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/24/2016

\brief   PNG texture importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Resource/sfResourceImporter.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"

namespace Sulfur
{
	
  class PngImporter : public IResourceImporter<Texture2D>
  {
  
  public:
    virtual void Load(BYTE *buffer, UINT32 bufferSize, Texture2D *resource) override;

  };
  
}