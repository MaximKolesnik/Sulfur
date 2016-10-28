#include "sfBoxCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
  BoxCollider::BoxCollider(void)
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