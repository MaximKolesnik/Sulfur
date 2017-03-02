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

      CalculateMass();

      SF_ASSERT(owner, "Owner is null");
      SF_ASSERT(m_transformHndl != SF_INV_HANDLE, "Transform handle is not set");
    }

    void ColliderData::SetMesh(const Mesh *mesh, const std::string &meshPath)
    {
      SF_ASSERT(mesh != nullptr, "Mesh is not set");

      m_geometry = GeometryMap::Instance()->GetGeometry(mesh, meshPath);
      CalculateMass();
    }

    void ColliderData::CalculateMass(void)
    {
      if (m_rbData)
      {
        Transform *trans = SF_GET_COMP_TYPE(Transform, m_transformHndl);

        if (m_rbData->m_state == RB_Static)
          m_rbData->m_invMass = 0;
        else
        {
          Real r, line;
          Vector3 scale;

          switch (m_type)
          {
          case CT_SPHERE:
            r = trans->GetScale().MaxAxisValue() * m_radius;
            m_rbData->m_invMass 
              = Real(1.0) / ( (Real(4.0) / 3) * SF_PI * r * r);
            break;

          case CT_CAPSULE:
            r = trans->GetScale().MaxAxisValue() * m_radius;
            line = trans->GetScale()[1] * m_lineLength;
            m_rbData->m_invMass
              = Real(1.0) / (SF_PI * r * r * ((Real(4.0) / 3) * r + line));
            break;

          case CT_MESH:
            if (m_geometry)
            {
              Real mass = 0;
              const auto &mesh = m_geometry->GetMesh();
              const auto &indices = mesh->GetIndices();
              const auto &verts = mesh->GetVertices();

              for (size_t i = 0; i < indices.size(); i += 3)
              {
                Vertex v1 = verts[indices[i]];
                Vertex v2 = verts[indices[i + 1]];
                Vertex v3 = verts[indices[i + 2]];
                Vector3 p1 = Vector3(v1.m_position[0], v1.m_position[1], v1.m_position[2]);
                Vector3 p2 = Vector3(v2.m_position[0], v2.m_position[1], v2.m_position[2]);
                Vector3 p3 = Vector3(v3.m_position[0], v3.m_position[1], v3.m_position[2]);

                mass += p1.Dot((p2 - p1).Cross(p3 - p1));
              }

              mass = mass / Real(6.0);
              m_rbData->m_invMass = Real(1.0) / mass;
            }
            break;
          }
        }
      }
    }
  }
}