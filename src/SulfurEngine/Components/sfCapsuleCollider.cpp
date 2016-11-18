#include "sfCapsuleCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
  CapsuleCollider::CapsuleCollider(void) : m_offset(Real(0.0), Real(0.0), Real(0.0)),
    m_radius(Real(0.5))
  {

  }

  CapsuleCollider::~CapsuleCollider(void)
  {

  }

  void CapsuleCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_CAPSULE);
  }

  CapsuleCollider* CapsuleCollider::Clone(void) const
  {
    SF_CRITICAL_ERR("CapsuleCollider clone not implemented");
    return nullptr;
  }

  void CapsuleCollider::Update(void)
  {

  }

  void CapsuleCollider::DrawDebug(DebugDraw *draw) const
  {

  }
}