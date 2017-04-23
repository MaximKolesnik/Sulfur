/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11Output.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Direct3D output (aka display)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfD3D11Wrapper.hpp"

namespace Sulfur
{
	
  class D3D11Output : public D3D11ResourceWrapper<IDXGIOutput, DXGI_OUTPUT_DESC>
  {

  public:
    virtual void Init(IDXGIOutput *output) override;
    virtual void Free() override;

    void GetDisplayModesForFormat(DXGI_FORMAT format, std::vector<DXGI_MODE_DESC>& modes);
    void CopyToTexture(ID3D11Texture2D *texture);
    void GetFrameStats(DXGI_FRAME_STATISTICS& stats);
    void WaitForVBlank();
  
  };
  
}