#include "sfPhysicsWorld.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Data\sfRigidBodyData.hpp"
#include "Data\sfColliderData.hpp"
#include "Error\sfError.hpp"
#include "Components\sfRigidBody.hpp"
#include "Components\sfTransform.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Integration\sfExplicitEuler.hpp"
#include "BroadPhase\sfBroadPhase.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"
#include "ColliderGeometry\sfGeometryMap.hpp"
#include "Collision\sfSAT.hpp"
#include "Modules\Physics\Constraint\sfConstraintSolver.hpp"

/******************************************************************************
Maxim TODO: Wrap all integrators
*******************************************************************************/
/******************************************************************************
Maxim TODO: Stop recomputing all Aabbs, when resting contacts are added
*******************************************************************************/

namespace Sulfur
{


  SF_DEFINE_TASK(IntegrateBodies)
  {
    for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
    {
      if (it.second->m_state == Physics::RB_Static)
        continue;

      Physics::ExplicitEuler integrator;
      integrator.Integrate(it.second);
    }
  } SF_END_DEFINE_TASK(IntegrateBodies);

  SF_DEFINE_TASK(BroadPhase)
  {
    for (auto &it : Physics::PhysicsWorld::Instance()->m_colliders)
    {
      Physics::PhysicsWorld::Instance()->m_broadPhase->UpdateProxy(it.second->m_proxy, it.second);
    }

    Physics::PhysicsWorld::Instance()->m_broadPhase->
      GetPossibleContacts(Physics::PhysicsWorld::Instance()->m_possiblePairs);

    Physics::PhysicsWorld::Instance()->m_broadPhase->DrawDebug(DebugDraw::Instance());
  } SF_END_DEFINE_TASK(BroadPhase);

  SF_DEFINE_TASK(NarrowPhase)
  {
    Physics::SAT sat;
    Physics::Contacts contacts;

    for (auto it : Physics::PhysicsWorld::Instance()->m_possiblePairs.m_results)
    {
      sat.BoxToBox(contacts, (Physics::ColliderData*)it.m_clientData0, (Physics::ColliderData*)it.m_clientData1);
    }

    Physics::ConstraintSolver solver;
    if (!contacts.empty())
      solver.Solve(contacts);
  } SF_END_DEFINE_TASK(NarrowPhase);

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
      rbTrans->SetRotation(rbData->m_orientation);
      //Clean up
      rbData->m_forces.ZeroOut();
    }

    Physics::PhysicsWorld::Instance()->m_possiblePairs.m_results.clear();
  } SF_END_DEFINE_TASK(PostAndCleanup);

  namespace Physics
  {
    const Vector3 PhysicsWorld::c_gravity = Vector3(Real(0.0), Real(-9.8), Real(0.0));
    const Real PhysicsWorld::c_allowedPenetration = Real(0.001);
    const Real PhysicsWorld::c_biasFactor = Real(0.1);

    PhysicsWorld::PhysicsWorld(void) : m_broadPhase(new BroadPhase())
    {

    }

    PhysicsWorld::~PhysicsWorld(void)
    {

    }

    void PhysicsWorld::Initialize(void)
    {
      GeometryMap::Instance()->Initialize();
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