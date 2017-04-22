/******************************************************************************/
/*!
\par     Sulfur
\file    sfTextureAtlas.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/7/2016

\brief   Texture atlas

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTextureAtlas.hpp"

namespace Sulfur
{

TextureAtlas::TextureAtlas()
  : m_device(nullptr), m_valid(false)
{
}

TextureAtlas::~TextureAtlas()
{
}

void TextureAtlas::Init(D3D11Device& device, UINT32 size, UINT32 initialArraySize)
{
  m_device = &device;
  m_textureSize = size;
  m_atlases.resize(initialArraySize);

  for (UINT32 i = 0; i < initialArraySize; ++i)
  {
    m_atlases[i].Offset[0] = m_atlases[i].Offset[1] = 0;
    m_atlases[i].Size[0] = m_atlases[i].Size[1] = m_textureSize;
    m_atlases[i].PixelData = nullptr;
    m_atlases[i].Left = nullptr;
    m_atlases[i].Right = nullptr;
  }
}

void TextureAtlas::Free()
{
  Clear();
}

void TextureAtlas::Clear()
{

}

void TextureAtlas::AddTexture(const std::string& texture)
{
  // LOAD TEXTURE
  BYTE *pixelData = nullptr;
  UINT32 width = 32;
  UINT32 height = 32;

  AtlasNode *node = nullptr;
  UINT32 index = 0;
  for (AtlasNode& root : m_atlases)
  {
    node = AddToNode(&root, pixelData, width, height);
    if (node) break;
    ++index;
  }

  if (!node)
  {
    m_atlases.push_back(AtlasNode{ { 0, 0 }, { m_textureSize, m_textureSize } , nullptr, nullptr, nullptr });
    node = AddToNode(&m_atlases.back(), pixelData, width, height);
  }

  AtlasData data = {
    index,
    { (node->Offset[0] + 0.5f) / m_textureSize, (node->Offset[1] + 0.5f) / m_textureSize },
    { (node->Size[0] - 1.0f) / m_textureSize, (node->Size[1] - 1.0f) / m_textureSize }
  };
  m_atlasData[texture] = data;

  m_valid = false;
}

const AtlasData* TextureAtlas::GetAtlasData(const std::string& texture) const
{
  auto& it = m_atlasData.find(texture);

  if (it == m_atlasData.end())
    return nullptr;

  return &it->second;
}

void TextureAtlas::SetPixel(D3D11Context& context, UINT32 slot)
{
  //Update();
  Texture2D::SetPixel(context, slot);
}

AtlasNode* TextureAtlas::AddToNode(AtlasNode *node, BYTE *pixelData, UINT32 width, UINT32 height)
{
  if (node->PixelData)
  {
    return nullptr;
  }
  else if (node->Left)
  {
    AtlasNode *result = AddToNode(node->Left, pixelData, width, height);
    if (!result) result = AddToNode(node->Right, pixelData, width, height);
    return result;
  }
  else
  {
    if (node->Size[0] == width && node->Size[1] == height)
    {
      node->PixelData = pixelData;
      return node;
    }
    else if (node->Size[0] < width || node->Size[1] < height)
    {
      return nullptr;
    }

    UINT32 dW = node->Size[0] - width;
    UINT32 dH = node->Size[1] - height;

    if (dW > dH)
    {
      // Split vertically
      node->Left = new AtlasNode{
        { node->Offset[0], node->Offset[1] },
        { width, node->Size[1] },
        nullptr, nullptr, nullptr
      };

      node->Right = new AtlasNode{
        { node->Offset[0] + width, node->Offset[1] },
        { node->Size[0] - width, node->Size[1] },
        nullptr, nullptr, nullptr
      };

      return AddToNode(node->Left, pixelData, width, height);
    }
    else
    {
      // Split horizontally
      node->Left = new AtlasNode{
        { node->Offset[0], node->Offset[1] },
        { node->Size[0], height },
        nullptr, nullptr, nullptr
      };

      node->Right = new AtlasNode{
        { node->Offset[0], node->Offset[1] + height },
        { node->Size[0], node->Size[1] - height },
        nullptr, nullptr, nullptr
      };

      return AddToNode(node->Left, pixelData, width, height);
    }
  }
}

}
