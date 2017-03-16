/******************************************************************************/
/*!
\par     Sulfur
\file    sfThirdPersonController.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   Third person controller component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfIEntity.hpp"
#include "Managers/TaskManager/sfTaskRegistry.hpp"

namespace Sulfur
{

SF_DECLARE_TASK(UpdateGameplay)

SF_REFLECTED_CLASS_DERIVED(ThirdPersonController, IEntity)

public:
  ThirdPersonController();

  virtual void Initialize(void) override final;
  virtual ThirdPersonController* Clone(void) const override final;
  virtual void Update(void) override final;
  virtual void DrawDebug(DebugDraw *draw) const override;

private:
  SF_PRIVATE_PROPERTY(Real, movementSpeed, MovementSpeed, "Movement Speed")
  SF_PRIVATE_PROPERTY(Real, jumpSpeed, JumpSpeed, "Jump Speed")
  SF_PRIVATE_PROPERTY(Real, rotationSpeed, RotationSpeed, "Rotation Speed")
  SF_PRIVATE_PROPERTY(Real, cameraDistance, CameraDistance, "Camera Distance")
  SF_PRIVATE_PROPERTY(Real, cameraHeight, CameraHeight, "Camera Height")
  SF_PRIVATE_PROPERTY(Real, cameraAngle, CameraAngle, "Camera Angle")

  Real m_cameraRotation;

};

}