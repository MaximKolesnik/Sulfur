/******************************************************************************/
/*!
\par     Sulfur
\file    sfThirdPersonController.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   Third person controller component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfThirdPersonController.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "sfTransform.hpp"

#include "Modules/Time/sfTime.hpp"
#include "Modules/Input/sfInputManager.hpp"

namespace Sulfur
{

ThirdPersonController::ThirdPersonController()
  : m_movementSpeed(1.0f), m_rotationSpeed(1.0f)
{

}

void ThirdPersonController::Initialize(void)
{

}

ThirdPersonController* ThirdPersonController::Clone(void) const
{
  ThirdPersonController *newTPC = SF_CREATE_COMP(ThirdPersonController);
  return newTPC;
}

void ThirdPersonController::Update(void)
{
  float dt = Time::Instance()->GetDt();
  InputManager *inputManager = InputManager::Instance();
  Transform *playerTransform = SF_GET_COMP_TYPE(Transform, m_owner);

  Real vertical = (Real)(inputManager->IsKeyDown('W') - inputManager->IsKeyDown('S'));
  Real horizontal = (Real)(inputManager->IsKeyDown('D') - inputManager->IsKeyDown('A'));

  Vector3 translation = vertical * Vector3::c_xAxis + horizontal * Vector3::c_zAxis;
  playerTransform->SetTranslation(playerTransform->GetTranslation() + translation * dt);
}

void ThirdPersonController::DrawDebug(DebugDraw *draw) const
{
}

}
