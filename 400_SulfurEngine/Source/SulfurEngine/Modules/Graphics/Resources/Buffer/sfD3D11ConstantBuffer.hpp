/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11ConstantBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Constant Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Buffer.hpp"

namespace Sulfur
{

class D3D11ConstantBuffer : public D3D11Buffer
{

public:
  void Init(D3D11Device& device, UINT32 bufferSize, const void *initialData = nullptr);

  template <typename BufferType>
  void Init(D3D11Device& device, const BufferType *initialData = nullptr)
  {
    Init(device, sizeof(BufferType), initialData);
  }

  void SetVertex(D3D11Context& context, UINT32 slot);
  void SetPixel(D3D11Context& context, UINT32 slot);
  void SetCompute(D3D11Context& context, UINT32 slot);

};

}