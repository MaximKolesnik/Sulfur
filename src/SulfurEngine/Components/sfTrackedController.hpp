/******************************************************************************/
/*!
\par     Sulfur
\file    sfTrackedController.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    02/25/2017

\brief   VR tracked controller component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "../Types/sfIEntity.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules/Graphics/Resources/Material/sfMaterial.hpp"

namespace Sulfur
{

enum Hand
{
  LEFT_HAND,
  RIGHT_HAND,
  NO_HAND
};

SF_REGISTER_ENUM_TYPE(Hand, "Left", "Right", "None")
SF_BASE_TYPE_SPECIALIZATION(Hand)
	
SF_REFLECTED_CLASS_DERIVED(TrackedController, IEntity)

public:
  TrackedController();
  ~TrackedController();

public:
  virtual void Initialize(void) override final;
  virtual TrackedController* Clone(void) const override final;
  virtual void Update(void) override final;

  bool IsReadyToRender() const;
  Mesh& GetMesh() const;
  Texture2D& GetTexture() const;

private:
  SF_PRIVATE_PROPERTY(Hand, hand, Hand, "Hand");

};
  
}