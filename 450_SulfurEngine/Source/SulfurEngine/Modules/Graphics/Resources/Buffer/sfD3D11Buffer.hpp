/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Buffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Graphics/Core/sfD3D11Wrapper.hpp"
#include "Modules/Graphics/Core/sfD3D11Device.hpp"
#include "Modules/Graphics/Core/sfD3D11Context.hpp"

namespace Sulfur
{
	
  class D3D11Buffer : public D3D11ResourceWrapper<ID3D11Buffer, D3D11_BUFFER_DESC>
  {
  
  public:
    void Init(D3D11Device& device, const D3D11_BUFFER_DESC& description, const void *initialData = nullptr);

    void SetData(D3D11Context& context, const void *data, UINT32 dataSize);

    template <typename BufferType>
    void SetData(D3D11Context& context, const BufferType& data)
    {
      SetData(context, (const void*)&data, sizeof(BufferType));
    }

    template <typename BufferType>
    void SetData(D3D11Context& context, const BufferType* data, UINT32 count)
    {
      SetData(context, (const void*)data, sizeof(BufferType) * count);
    }

  };
  
}