/******************************************************************************/
/*!
\par     Sulfur
\file    sfCapsuleCollider.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfCapsuleCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  CapsuleCollider::CapsuleCollider(void) : m_offset(Real(0.0), Real(0.0), Real(0.0)),
    m_radius(Real(0.5)), m_centerLineLength(Real(1.0))
  {

  }

  CapsuleCollider::~CapsuleCollider(void)
  {
    Physics::PhysicsWorld::Instance()->RemoveCollider(this->m_owner, this->m_hndl);
  }

  void CapsuleCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_CAPSULE);
  }

  CapsuleCollider* CapsuleCollider::Clone(void) const
  {
    CapsuleCollider *newCapsule = SF_CREATE_COMP(CapsuleCollider);

    newCapsule->m_offset = m_offset;
    newCapsule->m_radius = m_radius;
    newCapsule->m_centerLineLength = m_centerLineLength;

    return newCapsule;
  }

  void CapsuleCollider::Update(void)
  {

  }

  void CapsuleCollider::DrawDebug(DebugDraw *draw) const
  {

  }

  const Vector3& CapsuleCollider::GetOffset(void) const
  {
    return m_offset;
  }

  const Real&  CapsuleCollider::GetRadius(void) const
  {
    return m_radius;
  }

  const Real&  CapsuleCollider::GetCenterLineLength(void) const
  {
    return m_centerLineLength;
  }

  void CapsuleCollider::SetOffset(const Vector3 &offset)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_offset = offset;
    m_offset = offset;
  }

  void CapsuleCollider::SetRadius(const Real &rad)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_radius = rad;
    m_radius = rad;
  }

  void CapsuleCollider::SetCenterLineLength(const Real &length)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_lineLength = length;
    m_centerLineLength = length;
  }
}