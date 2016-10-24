#pragma once

#include "Types\sfTypes.hpp"
#include "Math\sfVector3.hpp"
#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  namespace Physics
  {
    enum ColliderType
    {
      CT_BOX = 0,
      CT_SPHERE,
      CT_NUM_TYPES
    };

    struct ColliderData
    {
      //Data should be initialized as soon as collider is added to PhysicsWorld
      ColliderData(HNDL colHndl, ColliderType type) : m_compHndl(colHndl), m_type(type) {}

      void Initialize(void);

      bool m_isGhost;
      Vector3 m_offset;
      union
      {
        Vector3 m_scale;
        Real m_radius; //Sphere
      };

      Proxy m_proxy; //Spatial partition proxy
      ColliderType m_type;

      HNDL m_transformHndl = SF_INV_HANDLE;
      HNDL m_compHndl = SF_INV_HANDLE; //Collider component hndl
    };
  }
}