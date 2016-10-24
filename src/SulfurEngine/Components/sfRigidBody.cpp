#include "sfRigidBody.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
/******************************************************************************
Maxim TODO: Calculate mass according to physics material and collider type 
            and size
*******************************************************************************/
  RigidBody::RigidBody(void) : IEntity(), m_velocity(0.0, 0.0, 0.0), 
    m_dynamicState(RB_Dynamic), m_invMass(1.0)
  {

  }

  RigidBody::~RigidBody()
  {

  }

  void RigidBody::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddRigidBody(this->m_hndl);
  }

  RigidBody* RigidBody::Clone(void) const
  {
    SF_CRITICAL_ERR("RigidBody Clonee not implemented");
    return nullptr;
  }

  void RigidBody::Update(void)
  {

  }

  void RigidBody::DrawDebug(DebugDraw *draw) const
  {

  }
}