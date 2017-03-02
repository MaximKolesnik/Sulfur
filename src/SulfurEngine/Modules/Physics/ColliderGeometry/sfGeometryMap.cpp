/******************************************************************************/
/*!
\par     Sulfur
\file    sfGeometryMap.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/17/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfGeometryMap.hpp"

namespace Sulfur
{
  namespace Physics
  {
    namespace
    {
      Vector3 VertPosition(const Vertex &v)
      {
        return Vector3(v.m_position[0], v.m_position[1], v.m_position[2]);
      }

      bool CheckGeometry(const ColliderGeometry *geometry)
      {
        //to exit infinite loop
        const int maxEdgesFace = 5000;

        const ColliderGeometry::EdgeList &edges = geometry->GetEdges();
        const ColliderGeometry::FaceList &faces = geometry->GetFaces();
        const ColliderGeometry::PlaneList &planes = geometry->GetPlanes();
        const ColliderGeometry::VertexList &vertices = geometry->GetVertices();

        //Check correct twin placement
        for (size_t i = 0; i < edges.size(); i += 2)
        {
          if (edges[i].m_twin != i + 1)
            return false;
          if (edges[i + 1].m_twin != i)
            return false;
        }

        //Check if edge loops around
        for (size_t i = 0; i < faces.size(); ++i)
        {
          UINT16 startEdge = faces[i].m_edge;

          UINT16 next = edges[startEdge].m_next;
          int safeCounter = 0;
          while (next != startEdge)
          {
            ++safeCounter;
            if (safeCounter >= maxEdgesFace)
              return false;

            if (next == edges[next].m_next)
              return false;

            next = edges[next].m_next;
          }
        }

        return true;
      }
    }

    GeometryMap::GeometryMap(void) : m_initialized(false)
    {

    }

    GeometryMap::~GeometryMap(void)
    {

    }

    void GeometryMap::Initialize(void)
    {
      /*SF_ASSERT(!m_initialized, "GeometryMap is already initialized");
      if (m_initialized)
        return;

      m_initialized = true;

      _BuildBoxGeometry();*/
    }

    const ColliderGeometry*
      GeometryMap::GetGeometry(const Mesh *mesh, const std::string &meshPath)
    {
      if (m_geometryCollection.find(meshPath) == m_geometryCollection.end())
        _BuildGeometry(mesh, meshPath);

      return m_geometryCollection[meshPath];
    }

    //void GeometryMap::_BuildBoxGeometry(void)
    //{
    //  SF_ASSERT(m_boxGeometry.m_vertices.empty(), "BoxGeometry is not empty");

    //  //Vertices
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(0.5), Real(-0.5)));   //0
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(0.5), Real(-0.5)));  //1
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(0.5), Real(0.5)));   //2
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(0.5), Real(0.5)));    //3

    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(-0.5), Real(-0.5)));  //4
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(-0.5), Real(-0.5))); //5
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(-0.5), Real(0.5)));  //6
    //  m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(-0.5), Real(0.5)));   //7

    //  //HalfEdges
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(2, 1, 0, 4));        //0    
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(17, 0, 1, 1));        //1
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(4, 3, 1, 4));         //2
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(19, 2, 2, 2));        //3
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(6, 5, 2, 4));        //4
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(20, 4, 3, 3));        //5
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(0, 7, 3, 4));        //6
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(22, 6, 0, 0));        //7
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(18, 9, 4, 1));         //8
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(11, 8, 5, 5));       //9
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(16, 11, 7, 0));        //10
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(13, 10, 4, 5));        //11
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(23, 13, 6, 3));       //12
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(15, 12, 7, 5));        //13
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(21, 15, 5, 2));        //14
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(9, 14, 6, 5));        //15
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(7, 17, 4, 0));           //16
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(8, 16, 0, 1));           //17
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(1, 19, 5, 1));           //18
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(14, 18, 1, 2));           //19
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(12, 21, 2, 3));          //20
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(3, 20, 6, 2));          //21
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(10, 23, 3, 0));          //22
    //  m_boxGeometry.m_edges.push_back(ColliderGeometry::HalfEdge(5, 22, 7, 3));          //23

    //  //Faces
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(10));
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(1));
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(3));
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(5));
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(0));
    //  m_boxGeometry.m_faces.push_back(ColliderGeometry::Face(9));

    //  //Planes
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(1, 0, 0), 
    //    m_boxGeometry.m_vertices[0]));
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(0, 0, -1),
    //    m_boxGeometry.m_vertices[0]));
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(-1, 0, 0),
    //    m_boxGeometry.m_vertices[1]));
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(0, 0, 1),
    //    m_boxGeometry.m_vertices[2]));
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(0, 1, 0),
    //    m_boxGeometry.m_vertices[0]));
    //  m_boxGeometry.m_planes.push_back(Geometry::Plane(Vector3(0, -1, 0),
    //    m_boxGeometry.m_vertices[4]));
    //}

    void GeometryMap::_BuildGeometry(const Mesh *mesh, const std::string &meshPath)
    {
      ColliderGeometry *geometry = new ColliderGeometry();
      const Mesh::VertexList &meshVerts = mesh->GetVertices();
      const Mesh::IndexList &indList = mesh->GetIndices();
      
      //Assign faces to respective normals
      TriList tris;
      size_t indCount = indList.size();
      for (int i = 0; i < indCount; i += 3)
      {
        const Vertex &vertex = meshVerts[indList[i]];
        Triangle tri;

        tri.m_index[0] = indList[i]; 
        tri.m_index[1] = indList[i + 1]; 
        tri.m_index[2] = indList[i + 2];

        Vector3 v1 = Vector3(vertex.m_position[0],
          vertex.m_position[1], vertex.m_position[2]);
        Vector3 v2 = Vector3(meshVerts[indList[i + 1]].m_position[0], 
          meshVerts[indList[i + 1]].m_position[1], meshVerts[indList[i + 1]].m_position[2]);
        Vector3 v3 = Vector3(meshVerts[indList[i + 2]].m_position[0],
          meshVerts[indList[i + 2]].m_position[1], meshVerts[indList[i + 2]].m_position[2]);

        Vector3 normal = (v3 - v1).Cross(v2 - v1);
        normal.Normalize();

        if (normal.Dot(v1 - Vector3(0, 0, 0)) < 0)
          normal = -normal;

        normal = normal.Rounded(4);
        auto &triVec = tris[normal];
        triVec.push_back(tri);
      }

      std::vector<IntermediateFace> faces;

      _MergeTriangles(faces, tris, meshVerts, indList);
      _GetReducedVertices(geometry->m_vertices, meshVerts);
      _ChangeFaceVertsToReduced(faces, geometry->m_vertices, meshVerts);
      _FillHalfEdgesAndFaces(geometry, faces);

      SF_ASSERT(CheckGeometry(geometry), "Geometry construction failed");

      _SetExtents(mesh, geometry);

      geometry->m_mesh = mesh;
      m_geometryCollection[meshPath] = geometry;
    }

    void GeometryMap::_MergeTriangles(std::vector<IntermediateFace> &faces, const TriList &tris,
      const Mesh::VertexList &meshVerts, const Mesh::IndexList &indList) const
    {
      _CreateIntermediateFaces(faces, tris);

      //Now align vertices into clockwise order
      //Sort by the spanning angle around centroid
      for (auto &interFace : faces)
      {
        SF_ASSERT(interFace.m_vertices.size() != 0, "Face is empty");

        Vector3 centroid;
        centroid.ZeroOut();

        //Calculate centroid
        for (const auto &vert : interFace.m_vertices)
        {
          Vertex v = meshVerts[vert];

          centroid += Vector3(v.m_position[0], v.m_position[1], v.m_position[2]);

          interFace.m_sortedVerts.push_back(SortedVertex(vert, vert));
        }

        centroid /= (Real)interFace.m_vertices.size();

        _GiftWrapping(meshVerts, interFace, centroid);
      }
    }

    void GeometryMap::_CreateIntermediateFaces(std::vector<IntermediateFace> &interFaces,
      const TriList &tris) const
    {
      //Create intermediate faces
      for (auto &tri : tris)
      {
        SF_ASSERT(tri.second.size() != 0, "Triangle is empty");

        IntermediateFace face;
        face.m_normal = tri.first;

        auto &triVec = tri.second;
        if (triVec.size() == 1)
        {
          face.m_vertices.insert(triVec[0].m_index[0]);
          face.m_vertices.insert(triVec[0].m_index[1]);
          face.m_vertices.insert(triVec[0].m_index[2]);
        }
        else
        {
          for (auto it : triVec)
          {
            face.m_vertices.insert(it.m_index[0]);
            face.m_vertices.insert(it.m_index[1]);
            face.m_vertices.insert(it.m_index[2]);
          }
        }

        interFaces.push_back(face);
      }
    }

    void GeometryMap::_GetReducedVertices(ColliderGeometry::VertexList &vertices,
      const Mesh::VertexList &meshVerts) const
    {
      std::unordered_set<Vector3> vertSet;

      for (auto &it : meshVerts)
      {
        Vector3 v(Vector3(it.m_position[0], it.m_position[1], it.m_position[2]));
        v.Round(4);
        vertSet.insert(v);
      }

      for (auto &vert : vertSet)
        vertices.push_back(vert);
    }

    void GeometryMap::_ChangeFaceVertsToReduced(std::vector<IntermediateFace> &faces,
      const ColliderGeometry::VertexList &vertList, const Mesh::VertexList &meshVerts) const
    {
      std::unordered_map<Vector3, UINT16> vecTOind;
      for (UINT16 i = 0; i < vertList.size(); ++i)
      {
        vecTOind[vertList[i]] = i;
      }

      UINT16 counter = 0;
      for (auto &face : faces)
      {
        for (auto &angIndPair : face.m_sortedVerts)
        {
          Vertex v = meshVerts[angIndPair.m_uniqueIndex];
          Vector3 vert = Vector3(v.m_position[0], v.m_position[1], v.m_position[2]);
          vert.Round(4);

          angIndPair.m_index = vecTOind[vert];
          angIndPair.m_uniqueIndex = counter;
          ++counter;
        }
      }

      return;
    }

    void GeometryMap::_FillHalfEdgesAndFaces(ColliderGeometry *geometry,
      const std::vector<IntermediateFace> &faces) const
    {
      ColliderGeometry::EdgeList &edges = geometry->m_edges;
      ColliderGeometry::FaceList &faceList = geometry->m_faces;
      ColliderGeometry::PlaneList &planeList = geometry->m_planes;

      //uinque/new pointer into edge list
      std::unordered_map<UINT16, UINT16> insertedEdges;

      //Count edges
      size_t numEdges = 0;
      for (const auto &face : faces)
        numEdges += face.m_sortedVerts.size();

      edges.resize(numEdges);

      UINT16 edgeCounter = 0;
      size_t faceCount = faces.size();
      for (size_t faceCounter = 0; faceCounter < faceCount; ++faceCounter)
      {
        size_t vertCount = faces[faceCounter].m_sortedVerts.size();
        const auto &vertices = faces[faceCounter].m_sortedVerts;
        for (int vert = 0; vert < vertCount; ++vert)
        {
          auto twinIterator = insertedEdges.find(faces[faceCounter].m_sortedVerts[vert].m_uniqueIndex);
          if (twinIterator != insertedEdges.end())
            continue;

          UINT16 twinFace = 0, twinOrig = 0, twinNext = 0, twinUnique = 0;
          UINT16 thisOrig = vertices[vert].m_index;
          UINT16 thisEnd = ((vert + 1 != vertCount) ? vertices[vert + 1] : vertices[0]).m_index;

          for (size_t i = 0; i < faceCount; ++i)
          {
            if (i == faceCounter) continue;

            if (_GetTwinEdgeInfo(faces[i], thisOrig, thisEnd, twinOrig, twinNext, twinUnique))
            {
              twinFace = (UINT16)i;
              break;
            }

            SF_ASSERT(i + 1 != faceCount, "Cannot construct EdgeTwin relation");
          }

          insertedEdges.insert( { vertices[vert].m_uniqueIndex, (UINT16)edgeCounter } );

          edges[edgeCounter].m_face = (UINT16)faceCounter;
          edges[edgeCounter].m_next = ((vert + 1 != vertCount) ? vertices[vert + 1] : vertices[0]).m_uniqueIndex;
          edges[edgeCounter].m_origin = thisOrig;
          edges[edgeCounter].m_twin = edgeCounter + 1;

          insertedEdges.insert({ twinUnique, (UINT16)(edgeCounter + 1) });

          edges[edgeCounter + 1].m_face = twinFace;
          edges[edgeCounter + 1].m_next = twinNext;
          edges[edgeCounter + 1].m_origin = twinOrig;
          edges[edgeCounter + 1].m_twin = edgeCounter;

          edgeCounter += 2;
        }
      }

      //Now restore next indecies
      for (int i = 0; i < numEdges; i += 2)
      {
        UINT16 e1Next = edges[i].m_next;
        edges[i].m_next = insertedEdges[e1Next];

        UINT16 e2Next = edges[i + 1].m_next;
        edges[i + 1].m_next = insertedEdges[e2Next];
      }

      for (size_t faceCounter = 0; faceCounter < faceCount; ++faceCounter)
      {
        Vector3 n = faces[faceCounter].m_normal;
        UINT16 edge = insertedEdges[faces[faceCounter].m_sortedVerts[0].m_uniqueIndex];
        Vector3 p = geometry->m_vertices[edges[edge].m_origin];

        planeList.push_back(Geometry::Plane(n, p));
        faceList.push_back(ColliderGeometry::Face(edge));
      }
    }


    bool GeometryMap::_GetTwinEdgeInfo(const IntermediateFace &interFace,
      UINT16 twinOfOrig, UINT16 twinOfEnd,UINT16 &origin, UINT16 &next,
      UINT16 &twinUnique) const
    {
      size_t numVerts = interFace.m_sortedVerts.size();
      for (size_t i = 0; i < numVerts; ++i)
      {
        UINT16 v1 = interFace.m_sortedVerts[i].m_index;
        UINT16 v2 = ((i + 1 != numVerts) 
          ? interFace.m_sortedVerts[i + 1] : interFace.m_sortedVerts[0]).m_index;

        /*SF_ASSERT(!(std::find(interFace.m_sortedVerts.begin(), interFace.m_sortedVerts.end(), 3) != interFace.m_sortedVerts.end()
          && std::find(interFace.m_sortedVerts.begin(), interFace.m_sortedVerts.end(), 71) != interFace.m_sortedVerts.end()), "");*/

        if ((v1 == twinOfEnd && v2 == twinOfOrig) || (v1 == twinOfOrig && v2 == twinOfEnd))
        {
          origin = v1;
          next = ((i + 1 != numVerts)
            ? interFace.m_sortedVerts[i + 1] : interFace.m_sortedVerts[0]).m_uniqueIndex;
          twinUnique = interFace.m_sortedVerts[i].m_uniqueIndex;
          return true;
        }
      }

      return false;
    }

    void GeometryMap::_SetExtents(const Mesh *mesh, ColliderGeometry *geometry)
    {
      geometry->m_extents = mesh->m_aabb.GetHalfSize();
    }

    void GeometryMap::_GiftWrapping(const Mesh::VertexList &meshVerts, 
      IntermediateFace &face, const Vector3 &centroid) const
    {
      const auto &input = face.m_sortedVerts;
      size_t numInput = input.size();
      std::vector<SortedVertex> output;

      Vertex v1 = meshVerts[input[0].m_index];

      //Find any extreme point
      Vector3 searchDir = VertPosition(v1) - centroid;
      UINT16 maxIndex = 0;
      Real maxDist = -SF_REAL_MAX;
      Real dist;

      for (UINT16 i = 1; i < numInput; ++i)
      {
        dist = searchDir.Dot(VertPosition(meshVerts[input[i].m_index]));
        if (dist > maxDist)
        {
          maxDist = dist;
          maxIndex = i;
        }
      }

      UINT16 currVert = maxIndex;
      UINT16 nextVert;

      do
      {
        output.push_back(SortedVertex(input[currVert].m_index, input[currVert].m_index));

        nextVert = (currVert + 1) % numInput;
        for (UINT16 i = 0; i < numInput; ++i)
        {
          if (i == currVert || i == nextVert)
            continue;

          if (_Orientation(meshVerts, face.m_normal, input[currVert].m_index, 
            input[nextVert].m_index, input[i].m_index))
            nextVert = i;
        }

        currVert = nextVert;
      } while (currVert != maxIndex);

      face.m_sortedVerts = output;
    }

    bool GeometryMap::_Orientation(const Mesh::VertexList &meshVerts, 
      const Vector3 &faceNormal, UINT16 p, UINT16 q, UINT16 r) const
    {
      Vector3 vp = VertPosition(meshVerts[p]);
      Vector3 vq = VertPosition(meshVerts[q]);
      Vector3 vr = VertPosition(meshVerts[r]);

      Vector3 pq = vq - vp;
      Vector3 pr = vr - vp;
      Vector3 n = pq.Cross(pr);

      return n.Dot(faceNormal) < 0;
    }
  }
}