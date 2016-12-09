/******************************************************************************/
/*!
\par     Sulfur
\file    sfBSPTree.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Math\Geometry\sfShapes.hpp"

namespace Sulfur
{
  typedef std::vector<Geometry::Triangle> TriangleList;

  class BspTreeQueryData
  {
  public:
    BspTreeQueryData();

    std::vector<Geometry::Triangle> mTriangles;
    int mDepth;
  };

  class BspTree
  {
  public:

    // Splits the triangle by the given plane into 4 different lists. The front and back side list should be self explanatory (using the proper clipping table described in class).
    // The coplanar front list should be added to when a triangle is coplanar and the normal points the same direction as the plane normal. When the normal points in the opposite
    // direction then the coplanar back list should be added to. Note: When more than 3 points need to be added to a side you should split your polygon into a triangle using a
    // triangle fan. That is (i0, i1, i2), (i0, i2, i3) in order to match my output.
    static void SplitTriangle(const Geometry::Plane& plane, 
      const Geometry::Triangle& tri, TriangleList& coplanarFront, 
      TriangleList& coplanarBack, TriangleList& front, TriangleList& back, Real epsilon);
    static Real CalculateScore(const TriangleList& triangles, size_t testIndex, 
      Real k, Real epsilon);

  private:
    static Vector3 GetTriangleNormal(const Geometry::Triangle &tri);
    static Vector3 IntersectEdgeandNormal(const Vector3 &a, const Vector3 &b, 
      const Geometry::Plane &plane);

  public:

    // Return the index of the triangle to use as the split plane. You should use the passed in
    // k-value in your heuristic to balance the cost of balancing the tree with splitting triangles.
    size_t PickSplitPlane(const TriangleList& triangles, Real k, Real epsilon);

    // Given a list of triangles, construct a bsp-tree. You should use the heuristic shown in class
    // with the given weight (k) to choose between less splits and a more balanced tree.
    void Construct(const TriangleList& triangles, Real k, Real epsilon);

    // Given a ray determine if and and what time the ray hits this tree. This should return the t-first value!
    bool RayCast(const Geometry::Ray& ray, Real& t, Real rayPlaneEpsilon, 
      Real triExpansionEpsilon, int debuggingIndex);

    // Just a simple helper to return all triangles in this tree.
    void AllTriangles(TriangleList& triangles) const;
    // Invert the tree. This requires swapping all positive and negative sides of the tree
    // (split planes, inside/outside pointers, triangle winding order).
    void Invert();
    // Clips all triangles in this tree against all of the split planes of the passed in tree.
    // Use the provided epsilon for triangle splitting.
    void ClipTo(BspTree* tree, Real epsilon);
    // Update this tree so that it is a union of the current tree with the passed in tree.
    void Union(BspTree* tree, Real k, Real epsilon);
    // Update this tree to be the intersection of the current tree with the passed in tree.
    void Intersection(BspTree* tree, Real k, Real epsilon);
    // Update this tree to be the subtraction of the other tree from this tree.
    // That is, when A = this and B = other compute A - B.
    void Subtract(BspTree* tree, Real k, Real epsilon);

    // Fillout the array of triangles with the contents of the tree. The tree should do a pre-order depth-first insertion into the array
    // (where pre-order means triangle in the node, then recurse down the front of the plane, then the back).
    // Note: When building the tree make sure to add the split triangle as the first on inside the node and then all other coplanar triangles in order afterwards.
    void FilloutData(std::vector<BspTreeQueryData>& results) const;

    // Add your implementation here
  private:
    struct BSPNode
    {
      BSPNode(const TriangleList &triList, int depth, BSPNode *front, BSPNode *back,
        const Geometry::Plane &splitPlane) : m_triangles(triList), m_depth(depth), m_front(front),
        m_back(back), m_splitPlane(splitPlane) {}

      void ClipTo(BSPNode *node, Real epsilon);

      TriangleList m_triangles;

      int m_depth;

      BSPNode *m_front;
      BSPNode *m_back;

      Geometry::Plane m_splitPlane;

      TriangleList ClipTriangles(const TriangleList &triList, Real epsilon);
      void ClipTriangles(const TriangleList &triangles, TriangleList &newList, Real epsilon);
    };

    void DeleteTree(void);
    void DeleteNode(BSPNode *node);
    BSPNode* BuildTree(const TriangleList &triangles, int depth, Real k, Real epsilon);
    void FilloutTreeData(const BSPNode *node, std::vector<BspTreeQueryData>& results) const;
    bool RayCast(const BSPNode *node, const Geometry::Ray& ray, Real& t, Real tMin,
      Real tMax) const;
    void GetTriangles(const BSPNode *node, TriangleList &triangles) const;
    void InvertNode(BSPNode *node);

    BSPNode *m_root;

    Real m_rayPlaneEpsilon;
    Real m_triExpansionEpsilon;
  };
}