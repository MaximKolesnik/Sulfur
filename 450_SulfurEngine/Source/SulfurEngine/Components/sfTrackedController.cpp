/******************************************************************************/
/*!
\par     Sulfur
\file    sfTrackedController.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    02/25/2017

\brief   VR tracked controller component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTrackedController.hpp"
#include "Modules/Vr/sfVrManager.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfMeshRenderer.hpp"

namespace Sulfur
{

SF_BASE_TYPE_SERIALIZATION_DEFAULT(Hand)

TrackedController::TrackedController()
  : m_hand(Hand::NO_HAND)
{

}

TrackedController::~TrackedController()
{

}

void TrackedController::Initialize()
{
}

TrackedController* TrackedController::Clone() const
{
  TrackedController *trackedController = SF_CREATE_COMP(TrackedController);
  trackedController->m_hand = m_hand;
  return trackedController;
}

void TrackedController::Update()
{
  VrManager *vr = VrManager::Instance();
  VrManager::Controller controller = (VrManager::Controller)m_hand;
}

bool TrackedController::IsReadyToRender() const
{
  return (m_hand != Hand::NO_HAND) && VrManager::Instance()->IsControllerConnected((VrManager::Controller)m_hand);
}

Mesh& TrackedController::GetMesh() const
{
  return VrManager::Instance()->GetControllerMesh((VrManager::Controller)m_hand);
}

Texture2D& TrackedController::GetTexture() const
{
  return VrManager::Instance()->GetControllerTexture((VrManager::Controller)m_hand);
}

}
