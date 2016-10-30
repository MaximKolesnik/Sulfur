/******************************************************************************/
/*!
\par     Sulfur
\file    sfScript.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/14/2016

\brief   Script component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfIEntity.hpp"

namespace Sulfur
{

SF_REFLECTED_CLASS_DERIVED(Script, IEntity)
  
public:
  Script();
  virtual void Initialize(void) override final;
  virtual Script* Clone(void) const override final;
  virtual void Update(void) override;

};
  
}