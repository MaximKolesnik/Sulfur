/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11IndexBuffer.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D 11 Index Buffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Buffer.hpp"

namespace Sulfur
{

class D3D11IndexBuffer : public D3D11Buffer
{

public:
  void Init(D3D11Device& device, UINT32 indexCount, const UINT32 *indices = nullptr);

  void Set(D3D11Context& context);

};

}