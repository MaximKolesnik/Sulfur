/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredData.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Deferred pipeline data

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  enum GBufferTargets
  {
    GBUFFER_DIFFUSE,
    GBUFFER_NORMAL_METALLIC,
    GBUFFER_WORLD_POSITION_ROUGHNESS,
    GBUFFER_VIEW_SPACE_POSITION_DEPTH,
    GBUFFER_VIEW_DIRECTION,
    GBUFFER_EMISSIVE,
    GBufferTargetCount
  };
  
}