/******************************************************************************/
/*!
\par     Sulfur
\file    sfMeshCollider.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    1/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfMeshCollider.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  MeshCollider::MeshCollider(void) : m_meshResource(nullptr),
    m_offset(Real(0.0), Real(0.0), Real(0.0)), m_scale(Real(1.0), Real(1.0), Real(1.0))
  {

  }

  MeshCollider::~MeshCollider(void)
  {
    Physics::PhysicsWorld::Instance()->RemoveCollider(this->m_owner, this->m_hndl);
  }

  void MeshCollider::Initialize(void)
  {
    Physics::PhysicsWorld::Instance()->AddCollider(this->m_owner, this->m_hndl,
      Physics::ColliderType::CT_SPHERE);
  }

  MeshCollider* MeshCollider::Clone(void) const
  {
    MeshCollider* newCol = SF_CREATE_COMP(MeshCollider);

    newCol->m_meshResource = m_meshResource;
    newCol->m_meshResourcePath = m_meshResourcePath;
    newCol->m_offset = m_offset;
    newCol->m_scale = m_scale;

    return newCol;
  }

  void MeshCollider::Update(void)
  {

  }

  void MeshCollider::DrawDebug(DebugDraw *draw) const
  {

  }

  void MeshCollider::SetColliderMesh(const std::string &meshName)
  {
    SetMesh(meshName);
  }
}