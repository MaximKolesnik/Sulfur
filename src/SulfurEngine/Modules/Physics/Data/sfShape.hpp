#pragma once

#include "Math\Geometry\sfShapes.hpp"

namespace Sulfur
{
  namespace Physics
  {
    typedef Geometry::LineSegment Edge;

    struct Shape
    {
      std::vector<Edge> m_localEdges;
      std::vector<Edge> m_worldEdges;

      std::vector<Vector3> m_localNormals;
      std::vector<Vector3> m_worldNormals;
    };
  }
}