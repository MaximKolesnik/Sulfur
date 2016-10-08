/******************************************************************************/
/*!
\par     Sulfur
\file    sfRenderTarget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Render target

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"
#include "Modules/Graphics/Resources/Texture/sfTexture2D.hpp"
#include "Modules/Graphics/Target/sfDepthBuffer.hpp"
#include "Math/sfVector4.hpp"
#include "Utils/sfClassUtils.hpp"

namespace Sulfur
{
	
  class RenderTarget : public D3D11ResourceArrayWrapper<ID3D11RenderTargetView, D3D11_RENDER_TARGET_VIEW_DESC>
  {
  
  public:
    void Init(D3D11Device& device, Texture2D& texture);
    void Init(D3D11Device& device, const D3D11_TEXTURE2D_DESC& description);
    virtual void Free() override;

    virtual void Resize(UINT32 width, UINT32 height);

    void Set(D3D11Context& context);
    void Set(D3D11Context& context, UINT32 start, UINT32 count);
    void Set(D3D11Context& context, DepthBuffer& depthBuffer);
    void Set(D3D11Context& context, DepthBuffer& depthBuffer, UINT32 start, UINT32 count);
    void Clear(D3D11Context& context, const Vector4& color = Vector4(0.5f, 0.5f, 0.5f, 1.0f));

    Texture2D* GetTexture();
    const Texture2D* GetTexture() const;

  protected:
    D3D11Device *m_device;
    Texture2D m_texture;

    SF_CALLBACK(OnResize, UINT32, UINT32)

  };
  
}