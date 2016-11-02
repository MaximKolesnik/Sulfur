#pragma once

#include "Math\sfVector3.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class ColliderGeometry
    {
    public:
      struct Edge
      {
        Edge(UINT32 v0, UINT32 v1) : m_v0(v0), m_v1(v1) {}

        UINT32 m_v0;
        UINT32 m_v1;
      };

      struct Face
      {
        Face(const Vector3 &normal, const std::initializer_list<UINT32> il) 
          : m_normal(normal), m_vertIndecies(il) {}

        std::vector<UINT32> m_vertIndecies;
        Vector3 m_normal;
      };

      typedef std::vector<Vector3> VertexList;
      typedef std::vector<Edge> EdgeList;
      typedef std::vector<Face> FaceList;

      ColliderGeometry(void) {}
      ~ColliderGeometry(void) {}

      const VertexList& GetVertices(void) const { return m_vertices; }
      const EdgeList& GetUniqueEdges(void) const { return m_uniqueEdges; }
      const FaceList& GetUniqueFaces(void) const { return m_uniqueFaces; }
      const FaceList& GetFaces(void) const { return m_faces; }
    private:
      friend class GeometryMap;

      VertexList m_vertices;
      EdgeList m_uniqueEdges;
      FaceList m_uniqueFaces;
      FaceList m_faces;
    };
  }
}