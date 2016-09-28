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
#include <fbxsdk.h>

namespace Sulfur
{

class FbxImporter : public IResourceImporter<Mesh>
{

public:
  FbxImporter();
  virtual ~FbxImporter();

  virtual void Load(BYTE *buffer, UINT32 bufferSize, Mesh *resource) override;

private:

  void LoadMesh(FbxScene *scene, FbxNode *node, Mesh *resource, double scale);

private:
  FbxManager *m_manager;

};

}