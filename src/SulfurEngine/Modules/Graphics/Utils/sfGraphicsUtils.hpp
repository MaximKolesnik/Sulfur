/******************************************************************************/
/*!
\par     Sulfur
\file    sfGraphicsUtils.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Graphics utility functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class GraphicsUtils
  {

  public:
    static UINT32 DxgiFormatBpp(DXGI_FORMAT format);
    static DXGI_FORMAT DxgiFormatFromTypeAndMask(D3D_REGISTER_COMPONENT_TYPE type, UINT32 mask);

  };
  
}