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
#include "Modules/Scene/sfSceneManager.hpp"
#include "sfTransform.hpp"

#include "Modules/Time/sfTime.hpp"
#include "Modules/Input/sfInputManager.hpp"

namespace Sulfur
{

ThirdPersonController::ThirdPersonController()
  : m_movementSpeed(1.0f), m_rotationSpeed(1.0f), m_cameraDistance(5.0f), m_cameraHeight(1.0f), m_cameraAngle(0.15f), m_cameraRotation(0.0f)
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
  HNDL cameraObjectHandle = SceneManager::Instance()->GetScene().GetCameraObject();
  if (cameraObjectHandle == SF_INV_HANDLE) return;

  Transform *camera = SF_GET_OBJECT(cameraObjectHandle)->GetComponent<Transform>();
  Transform *player = SF_GET_COMP_TYPE(Transform, m_owner);

  float dt = Time::Instance()->GetDt();
  InputManager *inputManager = InputManager::Instance();

  Real vertical = (Real)(inputManager->IsKeyDown('W') - inputManager->IsKeyDown('S'));
  Real horizontal = (Real)(inputManager->IsKeyDown('D') - inputManager->IsKeyDown('A'));
  m_cameraRotation -= inputManager->MouseDeltaX() * m_rotationSpeed * 0.005f;

  Quaternion quat; quat.SetEuler(0.0f, 0.0f, m_cameraRotation);
  Quaternion quatDown; quatDown.SetEuler(m_cameraAngle, 0.0f, 0.0f);
  Vector3 direction = quat.GetMatrix3() * Vector3::c_zAxis;
  Vector3 translation = vertical * direction + horizontal * camera->GetWorldRight();

  player->SetTranslation(player->GetTranslation() + translation * dt * m_movementSpeed);  
  camera->SetRotation(quat * quatDown);
  camera->SetTranslation(player->GetTranslation() + Vector3::c_yAxis * m_cameraHeight - direction * m_cameraDistance);
}

void ThirdPersonController::DrawDebug(DebugDraw *draw) const
{
}

}
