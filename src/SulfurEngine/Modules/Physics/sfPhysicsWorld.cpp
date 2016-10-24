#include "sfPhysicsWorld.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Data\sfRigidBodyData.hpp"
#include "Error\sfError.hpp"
#include "Components\sfRigidBody.hpp"
#include "Components\sfTransform.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Integration\sfExplicitEuler.hpp"
#include "BroadPhase\sfBroadPhase.hpp"

/******************************************************************************
Maxim TODO: Wrap all integrators
*******************************************************************************/

namespace Sulfur
{
  SF_DEFINE_TASK(IntegrateBodies)
  {
    for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
    {
      Physics::ExplicitEuler integrator;
      integrator.Integrate(it.second);
    }
  } SF_END_DEFINE_TASK(IntegrateBodies);

  SF_DEFINE_TASK(PostAndCleanup)
  {
    for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
    {
      Physics::RigidBodyData *rbData = it.second;
      RigidBody *rbComp = SF_GET_COMP_TYPE(RigidBody, rbData->m_compHndl);
      Transform *rbTrans = SF_GET_OBJECT(rbComp->GetOwner())->GetComponent<Transform>();

      //Post data
      rbComp->SetVelocity(rbData->m_velocity);
      rbTrans->SetTranslation(rbData->m_position);

      //Clean up
      rbData->m_forces.ZeroOut();
    }
  } SF_END_DEFINE_TASK(PostAndCleanup);

  namespace Physics
  {
    const Vector3 PhysicsWorld::c_gravity = Vector3(Real(0.0), Real(-9.8), Real(0.0));

    PhysicsWorld::PhysicsWorld(void) : m_broadPhase(new BroadPhase())
    {

    }

    PhysicsWorld::~PhysicsWorld(void)
    {

    }

    void PhysicsWorld::AddRigidBody(HNDL rbHndl)
    {
      SF_ASSERT(m_rigidBodies.find(rbHndl) == m_rigidBodies.end(),
        "RigidBody is already tracked by PhysicsWorld");

      RigidBodyData *rbData = new RigidBodyData(rbHndl);
      rbData->Initialize();

      m_rigidBodies.insert({ rbHndl, rbData });
    }

    void PhysicsWorld::RemoveRigidBody(HNDL rbHndl)
    {
      SF_ASSERT(m_rigidBodies.find(rbHndl) != m_rigidBodies.end(),
        "RigidBody is not tracked by PhysicsWorld");

      delete m_rigidBodies[rbHndl];
      m_rigidBodies.erase(rbHndl);
    }

    void PhysicsWorld::AddCollider(HNDL colHndl, ColliderType type)
    {
      SF_ASSERT(m_colliders.find(colHndl) == m_colliders.end(),
        "Collider is already tracked by PhysicsWorld");

      ColliderData *colliderData = new ColliderData(colHndl, type);
      colliderData->Initialize();

      m_broadPhase->AddProxy(colliderData->m_proxy, colliderData);
      m_colliders.insert({ colHndl, colliderData });
    }

    void PhysicsWorld::RemoveCollider(HNDL colHndl)
    {
      SF_ASSERT(m_colliders.find(colHndl) != m_colliders.end(),
        "Collider is not tracked by PhysicsWorld");

      m_broadPhase->RemoveProxy(m_colliders[colHndl]->m_proxy);

      delete m_colliders[colHndl];
      m_colliders.erase(colHndl);
    }

    /*void PhysicsWorld::_UpdateRBData(HNDL rbHndl)
    {
      SF_ASSERT(m_rigidBodies.find(rbHndl) != m_rigidBodies.end(),
        "RigidBody is not tracked by PhysicsWorld");

      RigidBodyData *rbData = m_rigidBodies[rbHndl];
      RigidBody *rbComp = ComponentFactory::Instance()->GetComponent<RigidBody>(rbHndl);
      Transform *rbTransform = SF_GET_COMP_TYPE(Transform, rbData->m_transformHndl);

      rbData->m_position = rbTransform->GetTranslation();
      rbData->m_velocity = rbComp->GetVelocity();
      rbData->m_invMass = rbComp->GetInverseMass();
    }*/
  }
}