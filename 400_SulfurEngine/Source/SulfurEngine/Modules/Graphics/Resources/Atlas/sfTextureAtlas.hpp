/******************************************************************************/
/*!
\par     Sulfur
\file    sfTextureAtlas.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/7/2016

\brief   Texture atlas

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"

namespace Sulfur
{
	
struct AtlasData
{
  UINT32 ArrayIndex;
  float Offset[2];
  float Size[2];
};

struct AtlasNode
{
  UINT32 Offset[2];
  UINT32 Size[2];
  BYTE *PixelData;

  AtlasNode *Left;
  AtlasNode *Right;
};

class TextureAtlas : public Texture2D
{
  
public:
  TextureAtlas();
  ~TextureAtlas();

  void Init(D3D11Device& device, UINT32 size, UINT32 initialArraySize = 1);
  void Free();

  void Clear();

  void AddTexture(const std::string& texture);
  const AtlasData* GetAtlasData(const std::string& texture) const;

private:
  AtlasNode* AddToNode(AtlasNode *node, BYTE *pixelData, UINT32 width, UINT32 height);

private:
  D3D11Device *m_device;
  UINT32 m_textureSize;

  bool m_valid;
  std::unordered_map<std::string, AtlasData> m_atlasData;
  std::vector<AtlasNode> m_atlases;

};
  
}