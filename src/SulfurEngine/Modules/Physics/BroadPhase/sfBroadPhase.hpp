#pragma once

#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  class DynamicAabbTree;

  namespace Physics
  {
    struct ColliderData;

    class BroadPhase
    {
    public:
      BroadPhase(void);

      void GetPossibleContacts(QueryResults &possibleContacts);
      void AddProxy(Proxy &proxy, ColliderData *data);
      void RemoveProxy(Proxy &proxy);
      void UpdateProxy(Proxy &proxy, ColliderData *data);

    private:
      DynamicAabbTree *m_space;
    };
  }
}