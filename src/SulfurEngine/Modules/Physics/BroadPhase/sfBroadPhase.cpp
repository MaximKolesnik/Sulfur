#include "sfBroadPhase.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
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
      SpatialPartitionData spData;
      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);

      Geometry::Aabb aabb = 
        Geometry::Aabb::BuildFromCenterAndHalfExtents(Vector3(0.0, 0.0, 0.0),
          Vector3(0.5, 0.5, 0.5));
      aabb.Transform(trans->GetScale(), trans->GetRotation().GetMatrix3(), trans->GetTranslation());

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
      Transform *trans = SF_GET_COMP_TYPE(Transform, data->m_transformHndl);
      SpatialPartitionData spData;

      Geometry::Aabb aabb =
        Geometry::Aabb::BuildFromCenterAndHalfExtents(Vector3(0.0, 0.0, 0.0),
          Vector3(0.5, 0.5, 0.5));
      aabb.Transform(trans->GetScale(), trans->GetRotation().GetMatrix3(), trans->GetTranslation());

      spData.m_aabb = aabb;
      spData.m_clientData = reinterpret_cast<void*>(data);
      m_space->UpdateProxy(proxy, spData);
    }

    void BroadPhase::DrawDebug(DebugDraw *draw) const
    {
      m_space->DrawDebug(draw);
    }
  }
}