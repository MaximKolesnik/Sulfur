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
#include "Modules/Resource/sfResourceManager.hpp"

namespace Sulfur
{

SF_REGISTER_RESOURCE_TYPE(Texture2D)

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
