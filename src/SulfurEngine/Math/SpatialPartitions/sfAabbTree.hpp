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
      Geometry::Aabb m_origAbbb;

      bool IsLeaf(void) const { return m_left == nullptr; }
      void SetChildren(Node *left, Node *right);

      Node *m_left;
      Node *m_right;
      Node *m_parent;
      int m_height;

      size_t m_lastAxis;
    };

    Node* _CreateNode(void *clientData, Geometry::Aabb &aabb, Geometry::Aabb &origAabb, Node *parent) const;
    Node*& _SelectNode(Node *nodeToInsert, Node *&left, Node *&right) const;
    void _UpdateNode(Node *node) const;
    void _UpdateParents(Node *node) const;

    void _Insert(Node *newNode, Node *&parent);
    void _StoreNodeData(std::vector<SpatialPartitionQueryData>& results, Node *node, int depth) const;
    void _UpdateHeights();
    void _SetHeight(Node *node) const;
    int _GetHeight(Node *node) const;

    //Balancing
    int _BalanceFactor(Node *node) const;
    void _BalanceTree(Node *&node);
    void _BalanceTreeUp(Node **node);
    void _RotateLeft(Node *&node);
    void _RotateRight(Node *&node);

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