/******************************************************************************/
/*!
\par     Sulfur
\file    sfRigidBody.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfRigidBody.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{
/******************************************************************************
Maxim TODO: Calculate mass according to physics material and collider type 
            and size
*******************************************************************************/
  RigidBody::RigidBody(void) : IEntity(), m_velocity(0.0, 0.0, 0.0), 
    m_dynamicState(Physics::RB_Dynamic), m_invMass(1.0)
  {
 
  }

  RigidBody::~RigidBody()
  {
    Physics::PhysicsWorld::Instance()->RemoveRigidBody(this->m_owner, this->m_hndl);
  }

  void RigidBody::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddRigidBody(this->m_owner, this->m_hndl);
  }

  RigidBody* RigidBody::Clone(void) const
  {
    RigidBody *newBody = SF_CREATE_COMP(RigidBody);

    newBody->m_velocity = m_velocity;
    newBody->m_dynamicState = m_dynamicState;
    newBody->m_angularVelocity = m_angularVelocity;

    return newBody;
  }

  void RigidBody::Update(void)
  {

  }

  void RigidBody::DrawDebug(DebugDraw *draw) const
  {

  }

  const Vector3& RigidBody::GetVelocity(void) const
  {
    return m_velocity;
  }

  const Vector3& RigidBody::GetAngularVelocity(void) const
  {
    return m_angularVelocity;
  }

  const Physics::RBDynamicState& RigidBody::GetDynamicState(void) const
  {
    return m_dynamicState;
  }

  void RigidBody::SetVelocity(const Vector3 &vel)
  {
    Physics::RigidBodyData* rbData =
      Physics::PhysicsWorld::Instance()->GetRigidBodyData(this->m_owner);

    SF_ASSERT(rbData->m_compHndl != SF_INV_HANDLE, "Invalid handle");

    rbData->m_velocity = vel;
    m_velocity = vel;
  }

  void RigidBody::SetAngularVelocity(const Vector3 &vel)
  {
    Physics::RigidBodyData* rbData =
      Physics::PhysicsWorld::Instance()->GetRigidBodyData(this->m_owner);

    SF_ASSERT(rbData->m_compHndl != SF_INV_HANDLE, "Invalid handle");

    rbData->m_angularVelocity = vel;
    m_angularVelocity = vel;
  }

  void RigidBody::SetDynamicState(const Physics::RBDynamicState &state)
  {
    Physics::RigidBodyData* rbData =
      Physics::PhysicsWorld::Instance()->GetRigidBodyData(this->m_owner);

    SF_ASSERT(rbData->m_compHndl != SF_INV_HANDLE, "Invalid handle");

    rbData->m_state = state;

    m_dynamicState = state;
  }
}