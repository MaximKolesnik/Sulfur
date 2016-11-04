#include "sfColliderData.hpp"
#include "Components\sfSphereCollider.hpp"
#include "Components\sfBoxCollider.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Error\sfError.hpp"
#include "Components\sfTransform.hpp"
#include "Components\sfRigidBody.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

/******************************************************************************
Maxim TODO: Handle the case when collider is attached before the rigid body
*******************************************************************************/

namespace Sulfur
{
  namespace Physics
  {
    void ColliderData::Initialize(void)
    {
      SF_ASSERT(m_compHndl != SF_INV_HANDLE, "Collider handle is not set");
      //Proxy
      m_proxy = Proxy();
      m_proxy.m_uIntProxy = m_compHndl;

      Object *owner = nullptr;

      //Sync comp data
      switch (m_type)
      {
      case ColliderType::CT_SPHERE:
      {
        SphereCollider *sphereCol = SF_GET_COMP_TYPE(SphereCollider, m_compHndl);
        m_isGhost = sphereCol->GetIsGhost();
        m_offset = sphereCol->GetOffset();
        m_radius = sphereCol->GetRadius();

        owner = SF_GET_OBJECT(sphereCol->GetOwner());
        m_transformHndl = owner->GetComponentHandle<Transform>();
      }
      break;
      case ColliderType::CT_BOX:
      {
        BoxCollider *boxCol = SF_GET_COMP_TYPE(BoxCollider, m_compHndl);
        m_isGhost = boxCol->GetIsGhost();
        m_offset = boxCol->GetOffset();
        m_scale = boxCol->GetScale();

        owner = SF_GET_OBJECT(boxCol->GetOwner());
        m_transformHndl = owner->GetComponentHandle<Transform>();
      }
      break;
      default:
        SF_CRITICAL_ERR("Unrecognized collider type");
      }

      SF_ASSERT(owner, "Owner is null");
      SF_ASSERT(m_transformHndl != SF_INV_HANDLE, "Transform handle is not set");

      if (owner->HasComponent<RigidBody>())
      {
        SF_ASSERT(Physics::PhysicsWorld::Instance()->m_rigidBodies.find(owner->GetComponentHandle<RigidBody>())
          != Physics::PhysicsWorld::Instance()->m_rigidBodies.end(), "RigidBody is not attached");

        m_rbData = Physics::PhysicsWorld::Instance()->m_rigidBodies[owner->GetComponentHandle<RigidBody>()];
      }

    }
  }
}