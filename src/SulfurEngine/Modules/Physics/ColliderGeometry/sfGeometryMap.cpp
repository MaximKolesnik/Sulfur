#include "sfGeometryMap.hpp"

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
      SF_ASSERT(!m_initialized, "GeometryMap is already initialized");
      if (m_initialized)
        return;

      m_initialized = true;

      _BuildBoxGeometry();
    }

    void GeometryMap::_BuildBoxGeometry(void)
    {
      SF_ASSERT(m_boxGeometry.m_vertices.empty(), "BoxGeometry is not empty");

      //Vertices
      m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(0.5), Real(-0.5)));   //0
      m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(0.5), Real(-0.5)));  //1
      m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(0.5), Real(0.5)));   //2
      m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(0.5), Real(0.5)));    //3

      m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(-0.5), Real(-0.5)));  //4
      m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(-0.5), Real(-0.5))); //5
      m_boxGeometry.m_vertices.push_back(Vector3(Real(-0.5), Real(-0.5), Real(0.5)));  //6
      m_boxGeometry.m_vertices.push_back(Vector3(Real(0.5), Real(-0.5), Real(0.5)));   //7

      //Faces 
      ColliderGeometry::Face f0123(Vector3(Real(0.0), Real(1.0), Real(0.0)),
        { 0, 1, 2, 3 });
      ColliderGeometry::Face f4567(Vector3(Real(0.0), Real(-1.0), Real(0.0)),
        { 4, 7, 6, 5 });
      ColliderGeometry::Face f0154(Vector3(Real(0.0), Real(0.0), Real(-1.0)),
        { 0, 4, 5, 1 });
      ColliderGeometry::Face f1265(Vector3(Real(-1.0), Real(0.0), Real(0.0)),
        { 2, 1, 5, 6 });
      ColliderGeometry::Face f2673(Vector3(Real(0.0), Real(0.0), Real(1.0)),
        { 2, 6, 7, 3 });
      ColliderGeometry::Face f0374(Vector3(Real(1.0), Real(0.0), Real(0.0)),
        { 0, 3, 7, 4 });

      m_boxGeometry.m_faces.push_back(f0123);
      m_boxGeometry.m_faces.push_back(f4567);
      m_boxGeometry.m_faces.push_back(f0154);
      m_boxGeometry.m_faces.push_back(f1265);
      m_boxGeometry.m_faces.push_back(f2673);
      m_boxGeometry.m_faces.push_back(f0374);

      //Unique Faces
      m_boxGeometry.m_uniqueFaces.push_back(f0123);
      m_boxGeometry.m_uniqueFaces.push_back(f2673);
      m_boxGeometry.m_uniqueFaces.push_back(f0374);

      //Unique Edges
      m_boxGeometry.m_uniqueEdges.push_back(ColliderGeometry::Edge(0, 1));
      m_boxGeometry.m_uniqueEdges.push_back(ColliderGeometry::Edge(0, 3));
      m_boxGeometry.m_uniqueEdges.push_back(ColliderGeometry::Edge(0, 4));
    }
  }
}