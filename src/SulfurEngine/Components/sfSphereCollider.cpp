#include "sfSphereCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

namespace Sulfur
{
  SphereCollider::SphereCollider(void) : m_isGhost(false), m_offset(Real(0.0), Real(0.0),
    Real(0.0)), m_radius(Real(0.5))
  {

  }

  SphereCollider::~SphereCollider(void)
  {

  }

  void SphereCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_SPHERE);
  }

  SphereCollider* SphereCollider::Clone(void) const
  {
    SF_CRITICAL_ERR("SphereCollider clone not implemented");
    return nullptr;
  }

  void SphereCollider::Update(void)
  {

  }

  void SphereCollider::DrawDebug(DebugDraw *draw) const
  {

  }
}