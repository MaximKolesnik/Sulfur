/******************************************************************************/
/*!
\par     Sulfur
\file    sfGeometryMap.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/17/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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