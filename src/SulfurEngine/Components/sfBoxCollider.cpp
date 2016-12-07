/******************************************************************************/
/*!
\par     Sulfur
\file    sfBoxCollider.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfBoxCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Factories\sfComponentFactory.hpp"
namespace Sulfur
{
  BoxCollider::BoxCollider(void) : m_offset(Real(0.0), Real(0.0), Real(0.0)),
    m_scale(Real(1.0), Real(1.0), Real(1.0))
  {

  }

  BoxCollider::~BoxCollider(void)
  {
    Physics::PhysicsWorld::Instance()->RemoveCollider(this->m_owner, this->m_hndl);
  }

  void BoxCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_BOX);
  }

  BoxCollider* BoxCollider::Clone(void) const
  {
    BoxCollider *newBox = SF_CREATE_COMP(BoxCollider);

    newBox->m_offset = m_offset;
    newBox->m_scale = m_scale;

    return newBox;
  }

  void BoxCollider::Update(void)
  {

  }

  void BoxCollider::DrawDebug(DebugDraw *draw) const
  {

  }

  const Vector3& BoxCollider::GetOffset(void) const
  {
    return m_offset;
  }

  const Vector3& BoxCollider::GetScale(void) const
  {
    return m_scale;
  }

  void BoxCollider::SetOffset(const Vector3& offset)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_offset = offset;
    m_offset = offset;
  }

  void BoxCollider::SetScale(const Vector3& scale)
  {
    Physics::ColliderData *cData = Physics::PhysicsWorld::Instance()->GetColliderData(m_owner);

    cData->m_scale = m_scale;
    m_scale = m_scale;
  }
}