/******************************************************************************/
/*!
\par     Sulfur
\file    sfBSPTree.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfBSPTree.hpp"
#include "Math\Geometry\sfGeometry.hpp"

#undef near
#undef far

namespace Sulfur
{
  BspTreeQueryData::BspTreeQueryData()
  {
    mDepth = 0;
  }

  void BspTree::SplitTriangle(const Geometry::Plane& plane, const Geometry::Triangle& tri, 
    TriangleList& coplanarFront, TriangleList& coplanarBack, TriangleList& front, 
    TriangleList& back, Real epsilon)
  {
    Geometry::IntersectionType::Type triType = 
      Geometry::PlaneTriangle(plane.m_data, tri.m_points[0],
      tri.m_points[1], tri.m_points[2], epsilon);

    switch (triType)
    {
    case Geometry::IntersectionType::Inside:
      front.push_back(tri);
      break;
    case Geometry::IntersectionType::Outside:
      back.push_back(tri);
      break;
    case Geometry::IntersectionType::Coplanar:
    {
      Vector3 triNormal = GetTriangleNormal(tri);

      if (Dot(plane.GetNormal(), triNormal) > Real(0.98))
        coplanarFront.push_back(tri);
      else
        coplanarBack.push_back(tri);
    }
    break;
    case Geometry::IntersectionType::Overlaps:
    {
      std::vector<Vector3> frontVerts, backVerts;

      Vector3 a = tri.m_points[0];
      Geometry::IntersectionType::Type aType = Geometry::PointPlane(a, plane.m_data, epsilon);

      //Fill verts
      for (int i = 1; i < 4; ++i)
      {
        Vector3 b;
        if (i == 3)
          b = tri.m_points[0];
        else
          b = tri.m_points[i];

        Geometry::IntersectionType::Type bType = Geometry::PointPlane(b, plane.m_data, epsilon);

        if (bType == Geometry::IntersectionType::Inside)
        {
          if (aType == Geometry::IntersectionType::Outside)
          {
            Vector3 intersection = IntersectEdgeandNormal(a, b, plane);
            frontVerts.push_back(intersection);
            backVerts.push_back(intersection);
          }

          frontVerts.push_back(b);
        }
        else if (bType == Geometry::IntersectionType::Outside)
        {
          if (aType == Geometry::IntersectionType::Inside)
          {
            Vector3 intersection = IntersectEdgeandNormal(a, b, plane);
            frontVerts.push_back(intersection);
            backVerts.push_back(intersection);
          }
          else if (aType == Geometry::IntersectionType::Coplanar)
            backVerts.push_back(a);

          backVerts.push_back(b);
        }
        else
        {
          frontVerts.push_back(b);
          if (aType == Geometry::IntersectionType::Outside)
            backVerts.push_back(b);
        }

        a = b;
        aType = bType;
      }

      //Create triangles
      if (frontVerts.size() > 3)
      {
        front.push_back(Geometry::Triangle(frontVerts[0], frontVerts[2], frontVerts[3]));
        front.push_back(Geometry::Triangle(frontVerts[0], frontVerts[1], frontVerts[2]));
      }
      else
        front.push_back(Geometry::Triangle(frontVerts[0], frontVerts[1], frontVerts[2]));

      if (backVerts.size() > 3)
      {
        back.push_back(Geometry::Triangle(backVerts[0], backVerts[2], backVerts[3]));
        back.push_back(Geometry::Triangle(backVerts[0], backVerts[1], backVerts[2]));
      }
      else
        back.push_back(Geometry::Triangle(backVerts[0], backVerts[1], backVerts[2]));
    }
    break;
    }
  }

  Real BspTree::CalculateScore(const TriangleList& triangles, size_t testIndex, Real k, Real epsilon)
  {
    const Geometry::Triangle &testTri = triangles[testIndex];
    Geometry::Plane testPlane(testTri.m_points[0], testTri.m_points[1], testTri.m_points[2]);

    if (testPlane.GetNormal() == Vector3::c_zero)
      return SF_REAL_MAX;

    int numFront = 0, numBack = 0, numOverlap = 0;

    size_t numTris = triangles.size();
    for (size_t i = 0; i < numTris; ++i)
    {
      if (i == testIndex)
        continue;

      const Geometry::Triangle &thisTri = triangles[i];
      Geometry::IntersectionType::Type triType = 
        Geometry::PlaneTriangle(testPlane.m_data, thisTri.m_points[0],
        thisTri.m_points[1], thisTri.m_points[2], epsilon);

      switch (triType)
      {
      case Geometry::IntersectionType::Inside:
        ++numFront;
        break;
      case Geometry::IntersectionType::Outside:
        ++numBack;
        break;
      case Geometry::IntersectionType::Overlaps:
        ++numOverlap;
        break;
      }
    }

    return k * numOverlap + (1 - k) * abs(numFront - numBack);
  }


  Vector3 BspTree::GetTriangleNormal(const Geometry::Triangle &tri)
  {
    Vector3 v1 = tri.m_points[1] - tri.m_points[0];
    Vector3 v2 = tri.m_points[2] - tri.m_points[0];

    Vector3 normal = Cross(v1, v2);

    return Normalized(normal);
  }

  Vector3 BspTree::IntersectEdgeandNormal(const Vector3 &a, const Vector3 &b,
    const Geometry::Plane &plane)
  {
    Vector3 ba = b - a;
    Vector3 planeNormal = plane.GetNormal();

    Real nDotA = Dot(a, planeNormal);
    Real nDotAB = Dot(ba, planeNormal);

    return a + (((plane.GetDistance() - nDotA) / nDotAB) * ba);
  }

  size_t BspTree::PickSplitPlane(const TriangleList& triangles, Real k, Real epsilon)
  {
    Real bestScore = SF_REAL_MAX;
    size_t bestIndex;

    size_t numTris = triangles.size();
    for (size_t i = 0; i < numTris; ++i)
    {
      Real newScore = CalculateScore(triangles, i, k, epsilon);
      if (newScore < bestScore)
      {
        bestScore = newScore;
        bestIndex = i;
      }
    }

    return bestIndex;
  }

  void BspTree::Construct(const TriangleList& triangles, Real k, Real epsilon)
  {
    m_root = BuildTree(triangles, 0, k, epsilon);
  }

  bool BspTree::RayCast(const Geometry::Ray& ray, Real& t, Real rayPlaneEpsilon, Real triExpansionEpsilon, int debuggingIndex)
  {
    m_rayPlaneEpsilon = rayPlaneEpsilon;
    m_triExpansionEpsilon = triExpansionEpsilon;

    return RayCast(m_root, ray, t, 0, SF_REAL_MAX);
  }

  void BspTree::AllTriangles(TriangleList& triangles) const
  {
    GetTriangles(m_root, triangles);
  }

  void BspTree::Invert()
  {
    InvertNode(m_root);
  }

  void BspTree::ClipTo(BspTree* tree, Real epsilon)
  {
    m_root->ClipTo(tree->m_root, epsilon);
  }

  void BspTree::Union(BspTree* tree, Real k, Real epsilon)
  {
    TriangleList newTriangles;

    this->m_root->ClipTo(tree->m_root, epsilon);
    tree->m_root->ClipTo(this->m_root, epsilon);

    InvertNode(tree->m_root);
    tree->m_root->ClipTo(this->m_root, epsilon);
    InvertNode(tree->m_root);

    this->AllTriangles(newTriangles);
    tree->AllTriangles(newTriangles);

    DeleteTree();
    this->Construct(newTriangles, k, epsilon);
  }

  void BspTree::Intersection(BspTree* tree, Real k, Real epsilon)
  {
    InvertNode(this->m_root);
    InvertNode(tree->m_root);

    this->Union(tree, k, epsilon);

    InvertNode(this->m_root);
  }

  void BspTree::Subtract(BspTree* tree, Real k, Real epsilon)
  {
    InvertNode(tree->m_root);

    this->Intersection(tree, k, epsilon);
  }

  void BspTree::FilloutData(std::vector<BspTreeQueryData>& results) const
  {
    FilloutTreeData(m_root, results);
  }

  void BspTree::BSPNode::ClipTo(BSPNode *node, Real epsilon)
  {
    m_triangles = node->ClipTriangles(m_triangles, epsilon);

    if (m_front)
      m_front->ClipTo(node, epsilon);
    if (m_back)
      m_back->ClipTo(node, epsilon);
  }

  TriangleList BspTree::BSPNode::ClipTriangles(const TriangleList &triList, Real epsilon)
  {
    TriangleList newList;

    ClipTriangles(triList, newList, epsilon);

    return newList;
  }

  void BspTree::BSPNode::ClipTriangles(const TriangleList &triangles, TriangleList &newList, Real epsilon)
  {
    TriangleList front, back;

    for (auto &it : triangles)
      SplitTriangle(m_splitPlane, it, front, back, front, back, epsilon);

    if (m_front)
      m_front->ClipTriangles(front, newList, epsilon);
    else
      newList.insert(newList.end(), front.begin(), front.end());

    if (m_back)
      m_back->ClipTriangles(back, newList, epsilon);
  }

  void BspTree::DeleteTree(void)
  {
    if (m_root)
      DeleteNode(m_root);

    m_root = nullptr;
  }

  void BspTree::DeleteNode(BSPNode *node)
  {
    if (node->m_front)
      DeleteNode(node->m_front);
    if (node->m_back)
      DeleteNode(node->m_back);

    delete node;
  }

  BspTree::BSPNode* BspTree::BuildTree(const TriangleList &triangles, int depth, Real k,
    Real epsilon)
  {
    if (triangles.empty())
      return nullptr;

    size_t splitIndex = PickSplitPlane(triangles, k, epsilon);
    const Geometry::Triangle &splitTri = triangles[splitIndex];
    Geometry::Plane splitPlane(splitTri.m_points[0], splitTri.m_points[1], splitTri.m_points[2]);

    TriangleList front, back, coplanar;

    size_t numTris = triangles.size();
    for (size_t i = 0; i < numTris; ++i)
    {
      const Geometry::Triangle &currTriangle = triangles[i];
      Geometry::IntersectionType::Type triType = 
        Geometry::PlaneTriangle(splitPlane.m_data, currTriangle.m_points[0],
        currTriangle.m_points[1], currTriangle.m_points[2], epsilon);

      switch (triType)
      {
      case Geometry::IntersectionType::Inside:
        front.push_back(currTriangle);
        break;
      case Geometry::IntersectionType::Outside:
        back.push_back(currTriangle);
        break;
      case Geometry::IntersectionType::Coplanar:
        coplanar.push_back(currTriangle);
        break;
      case Geometry::IntersectionType::Overlaps:
        SplitTriangle(splitPlane, currTriangle, coplanar, coplanar, front, back, epsilon);
        break;
      }
    }

    BSPNode *frontNode = BuildTree(front, depth + 1, k, epsilon);
    BSPNode *backNode = BuildTree(back, depth + 1, k, epsilon);

    return new BSPNode(coplanar, depth, frontNode, backNode, splitPlane);
  }

  void BspTree::FilloutTreeData(const BSPNode *node, std::vector<BspTreeQueryData>& results) const
  {
    if (!node)
      return;

    BspTreeQueryData data;

    data.mDepth = node->m_depth;
    data.mTriangles = node->m_triangles;

    results.push_back(data);

    FilloutTreeData(node->m_front, results);
    FilloutTreeData(node->m_back, results);
  }

  bool BspTree::RayCast(const BSPNode *node, const Geometry::Ray& ray, Real& t, Real tMin,
    Real tMax) const
  {
    bool result = false;

    if (!node)
      return result;

    Real dotRP = Dot(ray.m_direction, node->m_splitPlane.GetNormal());
    Real tEpsilon = abs(m_rayPlaneEpsilon / dotRP);

    Vector3 clippedStart = ray.m_start + tMin * ray.m_direction;

    Geometry::IntersectionType::Type rayType = 
      Geometry::PointPlane(clippedStart, node->m_splitPlane.m_data, m_rayPlaneEpsilon);

    Real tPlane;
    Geometry::RayPlane(clippedStart, ray.m_direction, node->m_splitPlane.m_data, 
      tPlane, m_triExpansionEpsilon);

    //Determine far/near
    const BSPNode *near, *far;
    if (rayType == Geometry::IntersectionType::Inside 
      || (rayType == Geometry::IntersectionType::Coplanar && dotRP < 0))
    {
      near = node->m_front;
      far = node->m_back;
    }
    else
    {
      near = node->m_back;
      far = node->m_front;
    }


    if (dotRP == 0)
    {
      if (rayType == Geometry::IntersectionType::Coplanar)
      {
        result |= RayCast(near, ray, t, tMin, tPlane);
        result |= RayCast(far, ray, t, tPlane, tMax);
      }
      else
        result |= RayCast(near, ray, t, tMin, tMax);
    }
    else if (tMin - tEpsilon < tPlane && tMax > tPlane - tEpsilon)
    {
      result |= RayCast(near, ray, t, tMin, tPlane);
      result |= RayCast(far, ray, t, tPlane, tMax);
    }
    else if (tPlane < 0)
      result |= RayCast(near, ray, t, tMin, tMax);
    else if (tMax < tPlane)
      result |= RayCast(near, ray, t, tMin, tMax);
    else if (tPlane > 0 && tPlane < tMin)
      result |= RayCast(far, ray, t, tPlane, tMax);


    Real bestT = SF_REAL_MAX;

    const TriangleList &triList = node->m_triangles;
    size_t numTris = triList.size();
    for (size_t i = 0; i < numTris; ++i)
    {
      const Geometry::Triangle &currTri = triList[i];
      Real newT;

      if (Geometry::RayTriangle(ray.m_start, ray.m_direction, currTri.m_points[0],
        currTri.m_points[1], currTri.m_points[2], newT, m_triExpansionEpsilon))
      {
        if (newT < t)
        {
          t = newT;
          result = true;
        }
      }
    }

    return result;
  }

  void BspTree::GetTriangles(const BSPNode *node, TriangleList &triangles) const
  {
    if (!node)
      return;

    triangles.insert(triangles.end(), node->m_triangles.begin(), node->m_triangles.end());

    GetTriangles(node->m_front, triangles);
    GetTriangles(node->m_back, triangles);
  }

  void BspTree::InvertNode(BSPNode *node)
  {
    if (!node)
      return;

    node->m_splitPlane.m_data = -node->m_splitPlane.m_data;

    TriangleList &tris = node->m_triangles;
    size_t numTris = tris.size();
    for (size_t i = 0; i < numTris; ++i)
    {
      Vector3 temp = tris[i].m_points[0];
      tris[i].m_points[0] = tris[i].m_points[1];
      tris[i].m_points[1] = temp;
    }

    BSPNode *temp = node->m_front;
    node->m_front = node->m_back;
    node->m_back = temp;

    InvertNode(node->m_front);
    InvertNode(node->m_back);
  }
}