/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Adapter.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D adapter (aka graphics card)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Wrapper.hpp"
#include "sfD3D11Output.hpp"

namespace Sulfur
{
	
  class D3D11Adapter : public D3D11ResourceWrapper<IDXGIAdapter, DXGI_ADAPTER_DESC>
  {

  public:
    typedef std::vector<D3D11Output> OutputList;

  public:
    virtual void Init(IDXGIAdapter *adapter) override;
    virtual void Free() override;

    const OutputList& GetOutputs() const;

  private:
    std::vector<D3D11Output> m_outputs;
  
  };
  
}