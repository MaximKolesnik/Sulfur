/******************************************************************************/
/*!
\par     Sulfur
\file    sfAabbTree.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "sfISpatialPartition.hpp"
#include "Math\Geometry\sfGeometry.hpp"

namespace Sulfur
{
  class DebugDraw;

  class DynamicAabbTree : public ISpatialPartition
  {
  public:
    DynamicAabbTree();
    ~DynamicAabbTree();

    // Spatial Partition Interface
    void CreateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void RemoveProxy(Proxy& proxy) override;

    void CastRay(const Geometry::Ray& ray, CastResults& results) override;
    void CastFrustum(const Geometry::Frustum& frustum, CastResults& results) override;

    void SelfQuery(QueryResults& results) override;

    void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;

    void DrawDebug(DebugDraw *draw) const;

    static const float m_fatteningFactor;

  private:

    struct Node
    {
      void *m_clientData;
      Geometry::Aabb m_aabb;

      bool IsLeaf(void) const { return m_left == nullptr; }
      void SetChildren(Node *left, Node *right);

      Node *m_left;
      Node *m_right;
      Node *m_parent;
      int m_height;
    };

    Node* _CreateNode(void *clientData, Node *parent) const;
    Node*& _SelectNode(Node *nodeToInsert, Node *&left, Node *&right) const;
    //void _UpdateNode(Node *node) const;
    //void _UpdateParents(Node *node) const;

    void _Insert(Node *newNode);
    void _StoreNodeData(std::vector<SpatialPartitionQueryData>& results, Node *node, int depth) const;
    Node* _BalanceNode(Node *node);
    Node* _RotateLeft(Node *&node);
    Node* _RotateRight(Node *&node);
    void _RemoveNode(Node *node);

    //Casting
    void _RayCast(CastResults &results, const Geometry::Ray &ray, Node *node) const;
    void _FrustumCast(const Geometry::Frustum& frustum, CastResults& results, Node *node) const;
    void _CollectLeafs(CastResults &results, Node *node) const;

    void _SelfQuery(QueryResults &results, Node *node) const;
    void _SelfQuery(QueryResults &results, Node *node1, Node *node2) const;

    void _DrawDebug(Node *node, DebugDraw *draw) const;

    Node *m_root;
  };
}