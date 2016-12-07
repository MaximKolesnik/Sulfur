#include "sfSphereCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Factories\sfComponentFactory.hpp"
namespace Sulfur
{
  SphereCollider::SphereCollider(void) : //m_isGhost(false), 
    m_offset(Real(0.0), Real(0.0), Real(0.0)), m_radius(Real(0.5))
  {

  }

  SphereCollider::~SphereCollider(void)
  {
    Physics::PhysicsWorld::Instance()->RemoveCollider(this->m_owner, this->m_hndl);
  }

  void SphereCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_SPHERE);
  }

  SphereCollider* SphereCollider::Clone(void) const
  {
    SphereCollider *newSphere = SF_CREATE_COMP(SphereCollider);

    newSphere->m_offset = m_offset;
    newSphere->m_radius = m_radius;

    return newSphere;
  }

  void SphereCollider::Update(void)
  {

  }

  void SphereCollider::DrawDebug(DebugDraw *draw) const
  {

  }

  const Vector3& SphereCollider::GetOffset(void) const
  {
    return m_offset;
  }

  const Real& SphereCollider::GetRadius(void) const
  {
    return m_radius;
  }

  void SphereCollider::SetOffset(const Vector3 &offset)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_offset = offset;
    m_offset = offset;
  }

  void SphereCollider::SetRadius(const Real &rad)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_radius = rad;
    m_radius = rad;
  }
}