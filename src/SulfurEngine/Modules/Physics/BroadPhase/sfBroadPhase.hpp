#pragma once

#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  class DynamicAabbTree;
  class DebugDraw;

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
      void DrawDebug(DebugDraw *draw) const;

    private:
      void _GetColliderTransformData(const ColliderData *data, Vector3 &translation, 
        Vector3 &scale, Quaternion &orient) const;

      DynamicAabbTree *m_space;
    };
  }
}