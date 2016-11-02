#pragma once

#include "Types\sfSingleton.hpp"
#include "sfColliderGeometry.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class GeometryMap
    {
      SF_SINGLETON(GeometryMap);
    public:
      void Initialize(void);

      const ColliderGeometry& GetBoxGeometry(void) const { return m_boxGeometry; }

    private:
      void _BuildBoxGeometry(void);

      bool m_initialized;
      ColliderGeometry m_boxGeometry;
    };
  }
}