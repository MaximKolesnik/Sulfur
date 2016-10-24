#include "sfColliderData.hpp"
#include "Components\sfSphereCollider.hpp"
#include "Components\sfBoxCollider.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Error\sfError.hpp"
#include "Components\sfTransform.hpp"

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

      //Sync comp data
      switch (m_type)
      {
      case ColliderType::CT_SPHERE:
      {
        SphereCollider *sphereCol = SF_GET_COMP_TYPE(SphereCollider, m_compHndl);
        m_isGhost = sphereCol->GetIsGhost();
        m_offset = sphereCol->GetOffset();
        m_radius = sphereCol->GetRadius();
        m_transformHndl = SF_GET_OBJECT(sphereCol->GetOwner())->GetComponentHandle<Transform>();
      }
      break;
      case ColliderType::CT_BOX:
      {
        BoxCollider *boxCol = SF_GET_COMP_TYPE(BoxCollider, m_compHndl);
        m_isGhost = boxCol->GetIsGhost();
        m_offset = boxCol->GetOffset();
        m_scale = boxCol->GetScale();
        m_transformHndl = SF_GET_OBJECT(boxCol->GetOwner())->GetComponentHandle<Transform>();
      }
      break;
      default:
        SF_CRITICAL_ERR("Unrecognized collider type");
      }
      
      SF_ASSERT(m_transformHndl != SF_INV_HANDLE, "Transform handle is not set");

    }
  }
}