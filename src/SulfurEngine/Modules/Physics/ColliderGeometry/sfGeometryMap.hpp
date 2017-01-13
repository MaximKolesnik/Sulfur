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
  class Mesh;

  namespace Physics
  {
    class GeometryMap
    {
      SF_SINGLETON(GeometryMap);
    public:
      void Initialize(void);
      const ColliderGeometry* GetGeometry(const Mesh *mesh, const std::string &meshPath);

    private:
      void _BuildBoxGeometry(void);
      void _BuildGeometry(const Mesh *mesh, const std::string &meshPath);

      bool m_initialized;
      std::unordered_map<std::string, ColliderGeometry*> m_geometryCollection;
    };
  }
}