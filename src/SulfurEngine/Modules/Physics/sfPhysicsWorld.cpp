/******************************************************************************/
/*!
\par     Sulfur
\file    sfPhysicsWorld.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfPhysicsWorld.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Data\sfRigidBodyData.hpp"
#include "Data\sfColliderData.hpp"
#include "Error\sfError.hpp"
#include "Components\sfRigidBody.hpp"
#include "Components\sfSphereCollider.hpp"
#include "Components\sfCapsuleCollider.hpp"
#include "Components\sfBoxCollider.hpp"
#include "Components\sfTransform.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Integration\sfExplicitEuler.hpp"
#include "BroadPhase\sfBroadPhase.hpp"
#include "ColliderGeometry\sfGeometryMap.hpp"
#include "Modules\Physics\Constraint\sfConstraintSolver.hpp"
#include "Modules\Physics\DebugDraw\sfDebug.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"
#include "Modules\Physics\Collision\sfCollision.hpp"
#include "Modules\Time\sfTime.hpp"

/******************************************************************************
Maxim TODO: Wrap all integrators
*******************************************************************************/
/******************************************************************************
Maxim TODO: Stop recomputing all Aabbs, when resting contacts are added
*******************************************************************************/

namespace Sulfur
{

  static void DrawContacts(Physics::Contacts &contacts)
  {
    for (auto it : contacts)
    {
      Matrix4 m;
      m.SetTransformation(Quaternion(Real(1.0), 0.0, 0.0, 0.0),
        Vector3(Real(0.2), Real(0.2), Real(0.2)), it.m_contactPoint);
      DebugDraw::Instance()->DrawBox(m);
      DebugDraw::Instance()->DrawVector(it.m_contactPoint, it.m_contactNormal);
    }
  }

  SF_DEFINE_TASK(SyncData)
  {
    for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
    {
      it.second->m_position = SF_GET_COMP_TYPE(Transform, it.second->m_transformHndl)->GetTranslation();
      if (it.second->m_compHndl != SF_INV_HANDLE)
      {
        it.second->Initialize();
      }
    }

    for (auto &it : Physics::PhysicsWorld::Instance()->m_colliders)
    {
      switch (it.second->m_type)
      {
      case Physics::CT_SPHERE:
      {
        SphereCollider *sphere = SF_GET_COMP_TYPE(SphereCollider, it.second->m_compHndl);
        it.second->m_offset = sphere->GetOffset();
        it.second->m_radius = sphere->GetRadius();
      }
      break;
      case Physics::CT_CAPSULE:
      {
        CapsuleCollider *capsule = SF_GET_COMP_TYPE(CapsuleCollider, it.second->m_compHndl);
        it.second->m_lineLength = capsule->GetCenterLineLength();
        it.second->m_offset = capsule->GetOffset();
        it.second->m_radius = capsule->GetRadius();
      }
      break;
      case Physics::CT_BOX:
      {
        BoxCollider *box = SF_GET_COMP_TYPE(BoxCollider, it.second->m_compHndl);
        it.second->m_offset = box->GetOffset();
        it.second->m_scale = box->GetScale();
      }
      break;
      }
    }
  } SF_END_DEFINE_TASK(SyncData);

  SF_DEFINE_TASK(IntegrateBodies)
  {
    if (!Time::Instance()->IsPaused())
    {
      for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
      {
        if (it.second->m_state == Physics::RB_Static)
          continue;

        Physics::ExplicitEuler integrator;
        integrator.Integrate(it.second);
      }
    }
  } SF_END_DEFINE_TASK(IntegrateBodies);

  SF_DEFINE_TASK(BroadPhase)
  {
    if (!Time::Instance()->IsPaused())
    {
      for (auto &it : Physics::PhysicsWorld::Instance()->m_colliders)
      {
        Physics::PhysicsWorld::Instance()->m_broadPhase->UpdateProxy(it.second->m_proxy, it.second);
      }

      Physics::PhysicsWorld::Instance()->m_broadPhase->
        GetPossibleContacts(Physics::PhysicsWorld::Instance()->m_possiblePairs);
    }
  } SF_END_DEFINE_TASK(BroadPhase);

  SF_DEFINE_TASK(NarrowPhase)
  {
    if (!Time::Instance()->IsPaused())
    {
      Physics::Contacts contacts;

      for (auto it : Physics::PhysicsWorld::Instance()->m_possiblePairs.m_results)
      {
        Collide(contacts, (Physics::ColliderData*)it.m_clientData0,
          (Physics::ColliderData*)it.m_clientData1);
      }

      Physics::ConstraintSolver solver;
      if (!contacts.empty())
      {
        DrawContacts(contacts);
        solver.Solve(contacts);
      }
    }
  } SF_END_DEFINE_TASK(NarrowPhase);

  SF_DEFINE_TASK(PostAndCleanup)
  {
    if (!Time::Instance()->IsPaused())
    {
      for (auto &it : Physics::PhysicsWorld::Instance()->m_rigidBodies)
      {
        Physics::RigidBodyData *rbData = it.second;
        if (rbData->m_compHndl == SF_INV_HANDLE)
          continue;

        RigidBody *rbComp = SF_GET_COMP_TYPE(RigidBody, rbData->m_compHndl);
        Transform *rbTrans = SF_GET_OBJECT(rbComp->GetOwner())->GetComponent<Transform>();

        //Post data
        rbComp->SetVelocity(rbData->m_velocity);
        rbComp->SetAngularVelocity(rbData->m_angularVelocity);
        rbTrans->SetTranslation(rbData->m_position);
        rbTrans->SetRotation(rbData->m_orientation);
        //Clean up
        rbData->m_forces.ZeroOut();
      }

      Physics::PhysicsWorld::Instance()->m_possiblePairs.m_results.clear();
    }
  } SF_END_DEFINE_TASK(PostAndCleanup);

  namespace Physics
  {
    const Vector3 PhysicsWorld::c_gravity = Vector3(Real(0.0), Real(-7), Real(0.0));
    const Real PhysicsWorld::c_allowedPenetration = Real(0.05);
    const Real PhysicsWorld::c_biasFactor = Real(0.2);

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

    void PhysicsWorld::AddRigidBody(HNDL owner, HNDL rbHndl)
    {
      auto rb = m_rigidBodies.find(owner);
      RigidBodyData *rbData;

      if (rb != m_rigidBodies.end())
      {
        SF_ASSERT(rb->second->m_compHndl == SF_INV_HANDLE, "RigidBody is already tracked");

        delete rb->second;
        m_rigidBodies.erase(rb);

        rbData = new RigidBodyData(rbHndl);
        rbData->Initialize();
      }
      else
      {
        rbData = new RigidBodyData(rbHndl);
        rbData->Initialize();
      }

      auto col = m_colliders.find(owner);
      if (col != m_colliders.end())
        col->second->m_rbData = rbData;

      m_rigidBodies.insert({ owner, rbData });
    }

    void PhysicsWorld::RemoveRigidBody(HNDL owner, HNDL rbHndl)
    {
      SF_ASSERT(m_rigidBodies.find(owner) != m_rigidBodies.end(),
        "RigidBody is not tracked by PhysicsWorld");
      SF_ASSERT(m_rigidBodies[owner]->m_compHndl != SF_INV_HANDLE,
        "Dummy rigidBody should not be removed");

      delete m_rigidBodies[owner];
      if (m_colliders.find(owner) != m_colliders.end())
        m_rigidBodies[owner] = _CreateDummyRigidBodyData(owner);
      else
        m_rigidBodies.erase(owner);
    }

    void PhysicsWorld::AddCollider(HNDL owner, HNDL colHndl, ColliderType type)
    {
      SF_ASSERT(m_colliders.find(owner) == m_colliders.end(),
        "Collider is already tracked by PhysicsWorld");

      ColliderData *colliderData = new ColliderData(colHndl, type);
      colliderData->Initialize();

      m_broadPhase->AddProxy(colliderData->m_proxy, colliderData);
      m_colliders.insert({ owner, colliderData });

      auto it = m_rigidBodies.find(owner);
      if (it != m_rigidBodies.end())
        colliderData->m_rbData = it->second;
      else
      {
        RigidBodyData *rb = _CreateDummyRigidBodyData(owner);

        m_rigidBodies[owner] = rb;
        colliderData->m_rbData = rb;
      }
    }

    void PhysicsWorld::RemoveCollider(HNDL owner, HNDL colHndl)
    {
      SF_ASSERT(m_colliders.find(owner) != m_colliders.end(),
        "Collider is not tracked by PhysicsWorld");

      m_broadPhase->RemoveProxy(m_colliders[owner]->m_proxy);

      delete m_colliders[owner];
      m_colliders.erase(owner);

      auto it = m_rigidBodies.find(owner);
      if (it != m_rigidBodies.end())
      {
        if (it->second->m_compHndl == SF_INV_HANDLE)
        {
          delete m_rigidBodies[owner];
          m_rigidBodies.erase(owner);
        }
      }
    }

    RigidBodyData* PhysicsWorld::GetRigidBodyData(HNDL owner)
    {
      if (m_rigidBodies.find(owner) == m_rigidBodies.end())
        return nullptr;

      return m_rigidBodies[owner];
    }

    ColliderData* PhysicsWorld::GetColliderData(HNDL owner)
    {
      if (m_colliders.find(owner) == m_colliders.end())
        return nullptr;

      return m_colliders[owner];
    }

    void PhysicsWorld::DebugDrawColliders(void) const
    {
      DrawColliders(m_colliders);
    }

    void PhysicsWorld::DebugDrawAABB(void) const
    {
      m_broadPhase->DrawDebug(DebugDraw::Instance());
    }

    RigidBodyData* PhysicsWorld::_CreateDummyRigidBodyData(HNDL owner) const
    {
      RigidBodyData *rb = new RigidBodyData(SF_INV_HANDLE);
      rb->m_transformHndl = SF_GET_OBJECT(owner)->GetComponentHandle<Transform>();
      if (rb->m_transformHndl != SF_INV_HANDLE)
        rb->m_position = SF_GET_COMP_TYPE(Transform, rb->m_transformHndl)->GetTranslation();
      else
        rb->m_position = Vector3::c_zero;

      rb->CalculateInertia();

      return rb;
    }
  }
}