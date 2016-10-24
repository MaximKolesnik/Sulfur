#pragma once

#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  class DynamicAabbTree;

  namespace Physics
  {
    class ColliderData;

    class BroadPhase
    {
    public:
      void GetPossibleContacts(QueryResults &possibleContacts);
      void AddProxy(Proxy &proxy, const ColliderData *data);
      void RemoveProxy(Proxy &proxy);
      void UpdateProxy(Proxy &proxy, const ColliderData *data);

    private:
      DynamicAabbTree *m_space;
    };
  }
}