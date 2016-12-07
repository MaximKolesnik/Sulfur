/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderGeometry.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/17/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Math\sfVector3.hpp"
#include "Math\Geometry\sfShapes.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class ColliderGeometry
    {
    public:
      
      struct HalfEdge
      {
        HalfEdge(UINT8 next, UINT8 twin, UINT8 origin, UINT8 face)
          : m_next(next), m_twin(twin), m_origin(origin), m_face(face) {}
        UINT8 m_next;
        UINT8 m_twin;
        UINT8 m_origin;
        UINT8 m_face;
      };

      struct Face
      {
        Face(UINT8 edge) : m_edge(edge) {}
        UINT8 m_edge;
      };

      typedef std::vector<Vector3> VertexList;
      typedef std::vector<HalfEdge> EdgeList;
      typedef std::vector<Face> FaceList;
      typedef std::vector<Geometry::Plane> PlaneList;

      ColliderGeometry(void) {}
      ~ColliderGeometry(void) {}

      const VertexList& GetVertices(void) const { return m_vertices; }
      const EdgeList& GetEdges(void) const { return m_edges; }
      const FaceList& GetFaces(void) const { return m_faces; }
      const PlaneList& GetPlanes(void) const { return m_planes; }

      size_t GetVertexCount(void) const { return m_vertices.size(); }
      size_t GetEdgeCount(void) const { return m_edges.size(); }
      size_t GetFaceCount(void) const { return m_faces.size(); }
      size_t GetPlaneCount(void) const { return m_planes.size(); }

      const Vector3& GetVertex(size_t index) const { return m_vertices[index]; }
      const HalfEdge& GetEdge(size_t index) const { return m_edges[index]; }
      const Face& GetFace(size_t index) const { return m_faces[index]; }
      const Geometry::Plane& GetPlane(size_t index) const { return m_planes[index]; }

      Vector3 GetSupport(const Vector3 &dir) const
      {
        int maxIndex = -1;
        Real maxDist = -SF_REAL_MAX;
        Real dist;

        for (size_t i = 0; i < m_vertices.size(); ++i)
        {
          dist = dir.Dot(m_vertices[i]);
          if (dist > maxDist)
          {
            maxDist = dist;
            maxIndex = (int)i;
          }
        }

        return m_vertices[maxIndex];
      }

    private:
      friend class GeometryMap;

      VertexList m_vertices;
      EdgeList m_edges;
      FaceList m_faces;
      PlaneList m_planes;
    };
  }
}