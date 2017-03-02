/******************************************************************************/
/*!
\par     Sulfur
\file    sfBroadPhase.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/1/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfBroadPhase.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Physics\ColliderGeometry\sfColliderGeometry.hpp"
#include "Components\sfTransform.hpp"
#include "Math\Geometry\sfShapes.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  namespace Physics
  {
    BroadPhase::BroadPhase(void) : m_space(new DynamicAabbTree())
    {

    }

    void BroadPhase::GetPossibleContacts(QueryResults &possibleContacts)
    {
      m_space->SelfQuery(possibleContacts);
    }

    void BroadPhase::AddProxy(Proxy &proxy, ColliderData *data)
    {
      Vector3 translation, scale;
      Quaternion orientation;
      SpatialPartitionData spData;

      _GetColliderTransformData(data, translation, scale, orientation);

      Geometry::Aabb aabb = 
        Geometry::Aabb::BuildFromCenterAndHalfExtents(Vector3(0.0, 0.0, 0.0),
          Vector3(0.5, 0.5, 0.5));
      aabb.Transform(scale, orientation.GetMatrix3(), translation);

      spData.m_aabb = aabb;
      spData.m_clientData = reinterpret_cast<void*>(data);
      m_space->CreateProxy(proxy, spData);
    }

    void BroadPhase::RemoveProxy(Proxy &proxy)
    {
      m_space->RemoveProxy(proxy);
    }

    void BroadPhase::UpdateProxy(Proxy &proxy, ColliderData *data)
    {
      Vector3 translation, scale;
      Quaternion orientation;
      SpatialPartitionData spData;

      _GetColliderTransformData(data, translation, scale, orientation);

      Geometry::Aabb aabb =
        Geometry::Aabb::BuildFromCenterAndHalfExtents(Vector3(0.0, 0.0, 0.0),
          Vector3(0.5, 0.5, 0.5));
      aabb.Transform(scale, orientation.GetMatrix3(), translation);

      spData.m_aabb = aabb;
      spData.m_clientData = reinterpret_cast<void*>(data);
      m_space->UpdateProxy(proxy, spData);
    }

    void BroadPhase::DrawDebug(DebugDraw *draw) const
    {
      m_space->DrawDebug(draw);
    }

    void BroadPhase::_GetColliderTransformData(const ColliderData *data, 
      Vector3 &translation, Vector3 &scale, Quaternion &orient) const
    {
      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);
      translation = trans->GetTranslation() + data->m_offset;
      orient = trans->GetRotation();

      switch (data->m_type)
      {
      case CT_MESH:
        scale = trans->GetScale() * data->m_scale;
        if (data->m_geometry)
          scale = scale * (2 * data->m_geometry->GetExtents());
        break;
      case CT_SPHERE:
        scale.Splat(trans->GetScale().MaxAxisValue() * data->m_radius * 2);
        break;
      case CT_CAPSULE:
        scale[0] = trans->GetScale()[0] * data->m_lineLength * 2;
        scale[1] = scale[2] = std::max(trans->GetScale()[1], trans->GetScale()[2])
          * data->m_radius * 2;
        break;
      default:
        SF_CRITICAL_ERR("Unrecognized collider type");
      }
    }
  }
}