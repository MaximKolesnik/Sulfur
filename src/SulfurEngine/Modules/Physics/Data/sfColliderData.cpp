/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderData.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfColliderData.hpp"
#include "Components\sfSphereCollider.hpp"
#include "Components\sfMeshCollider.hpp"
#include "Components\sfCapsuleCollider.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Error\sfError.hpp"
#include "Components\sfTransform.hpp"
#include "Components\sfRigidBody.hpp"
#include "Modules\Physics\ColliderGeometry\sfGeometryMap.hpp"

#include "Modules\Graphics\Scene\sfMesh.hpp"

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
        //m_isGhost = sphereCol->GetIsGhost();
        m_offset = sphereCol->GetOffset();
        m_radius = sphereCol->GetRadius();

        owner = SF_GET_OBJECT(sphereCol->GetOwner());
        m_transformHndl = owner->GetComponentHandle<Transform>();
      }
      break;
      case ColliderType::CT_MESH:
      {
        MeshCollider *meshCol = SF_GET_COMP_TYPE(MeshCollider, m_compHndl);
        //m_isGhost = boxCol->GetIsGhost();
        m_offset = meshCol->GetOffset();
        m_scale = meshCol->GetScale();
        
        owner = SF_GET_OBJECT(meshCol->GetOwner());
        m_transformHndl = owner->GetComponentHandle<Transform>();
      }
      break;
      case ColliderType::CT_CAPSULE:
      {
        CapsuleCollider *capCol = SF_GET_COMP_TYPE(CapsuleCollider, m_compHndl);
        //m_isGhost = capCol->GetIsGhost();
        m_radius = capCol->GetRadius();
        m_offset = capCol->GetOffset();
        m_lineLength = capCol->GetCenterLineLength();

        owner = SF_GET_OBJECT(capCol->GetOwner());
        m_transformHndl = owner->GetComponentHandle<Transform>();
      }
        break;
      default:
        SF_CRITICAL_ERR("Unrecognized collider type");
      }

      SF_ASSERT(owner, "Owner is null");
      SF_ASSERT(m_transformHndl != SF_INV_HANDLE, "Transform handle is not set");
    }

    void ColliderData::SetMesh(const Mesh *mesh, const std::string &meshPath)
    {
      SF_ASSERT(mesh != nullptr, "Mesh is not set");

      //m_geometry = 
    }
  }
}