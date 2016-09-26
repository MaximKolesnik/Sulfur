/******************************************************************************/
/*!
\par     Sulfur
\file    sfFbxImporter.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/24/2016

\brief   FBX model importer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Resource/sfResourceImporter.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"

namespace Sulfur
{

class FbxImporter : public IResourceImporter<Mesh>
{

public:
  virtual void Load(BYTE *buffer, UINT32 bufferSize, Mesh *resource) override;

};

}