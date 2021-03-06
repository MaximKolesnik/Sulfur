/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereCollider.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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
}