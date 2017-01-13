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
#include "Modules\Graphics\Scene\sfMesh.hpp"

namespace Sulfur
{
  namespace Physics
  {
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
      const Mesh::VertexList& meshVerts = mesh->GetVertices();

      
    }
  }
}