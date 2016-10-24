#include "sfBroadPhase.hpp"
#include "Math\SpatialPartitions\sfAabbTree.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void BroadPhase::GetPossibleContacts(QueryResults &possibleContacts)
    {
      m_space->SelfQuery(possibleContacts);
    }

    void BroadPhase::AddProxy(Proxy &proxy, const ColliderData *data)
    {
      SpatialPartitionData spData;
      spData.m_aabb = 
      m_space->CreateProxy(proxy, data);
    }

    void BroadPhase::RemoveProxy(Proxy &proxy)
    {
      m_space->RemoveProxy(proxy);
    }

    void BroadPhase::UpdateProxy(Proxy &proxy, const ColliderData *data)
    {
      m_space->UpdateProxy(proxy, data);
    }
  }
}