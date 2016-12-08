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
}