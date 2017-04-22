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
#include "Modules\Graphics\Scene\sfMesh.hpp"

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

      struct Triangle
      {
        UINT32 m_index[3];
      };

      struct SortedVertex
      {
        SortedVertex(UINT16 index, UINT16 uniqueIndex)
          : m_index(index), m_uniqueIndex(uniqueIndex) {}

        UINT16 m_index;
        UINT16 m_uniqueIndex;
      };

      struct IntermediateFace
      {
        Vector3 m_normal;
        std::unordered_set<UINT16> m_vertices;
        std::vector<SortedVertex> m_sortedVerts;
      };

      typedef std::unordered_map<Vector3, std::vector<Triangle> > TriList;

      void _BuildBoxGeometry(void);
      void _BuildGeometry(const Mesh *mesh, const std::string &meshPath);

      void _MergeTriangles(std::vector<IntermediateFace> &faces, const TriList &tris,
        const Mesh::VertexList &meshVerts, const Mesh::IndexList &indList) const;
      void _CreateIntermediateFaces(std::vector<IntermediateFace> &interFaces,
        const TriList &tris) const;

      void _GetReducedVertices(ColliderGeometry::VertexList &vertices,
        const Mesh::VertexList &meshVerts) const;
      void _ChangeFaceVertsToReduced(std::vector<IntermediateFace> &faces,
        const ColliderGeometry::VertexList &vertList, const Mesh::VertexList &meshVerts) const;

      void _FillHalfEdgesAndFaces(ColliderGeometry *geometry,
        const std::vector<IntermediateFace> &faces) const;

      bool _GetTwinEdgeInfo(const IntermediateFace &interFace,
        UINT16 twinOfOrig, UINT16 twinOfEnd, UINT16 &origin, UINT16 &next,
        UINT16 &twinUnique) const;

      void _SetExtents(const Mesh *mesh, ColliderGeometry *geometry);

      void _GiftWrapping(const Mesh::VertexList &meshVerts, 
        IntermediateFace &face, const Vector3 &centroid) const;
      bool _Orientation(const Mesh::VertexList &meshVerts, const Vector3 &faceNormal,
        UINT16 p, UINT16 q, UINT16 r) const;

      bool m_initialized;
      std::unordered_map<std::string, ColliderGeometry*> m_geometryCollection;
    };
  }
}