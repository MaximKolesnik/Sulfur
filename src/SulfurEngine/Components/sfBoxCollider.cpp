#include "sfBoxCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
  BoxCollider::BoxCollider(void) : m_offset(Real(0.0), Real(0.0), Real(0.0)),
    m_scale(Real(1.0), Real(1.0), Real(1.0))
  {

  }

  BoxCollider::~BoxCollider(void)
  {

  }

  void BoxCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_hndl, Physics::ColliderType::CT_BOX);
  }

  BoxCollider* BoxCollider::Clone(void) const
  {
    SF_CRITICAL_ERR("BoxCollider clone not implemented");
    return nullptr;
  }

  void BoxCollider::Update(void)
  {

  }

  void BoxCollider::DrawDebug(DebugDraw *draw) const
  {

  }
}