/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeferredLightPass.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   Node for rendering the light pass using a gbuffer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../sfRenderNode.hpp"

namespace Sulfur
{

class DeferredLightPass : public RenderNode
{

public:
  DeferredLightPass(D3D11Device& device);
  ~DeferredLightPass();

  virtual void Process() override;


};

}