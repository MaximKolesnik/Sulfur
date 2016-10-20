#pragma once

#include "sfISpatialPartition.hpp"
#include "Math\Geometry\sfGeometry.hpp"

namespace Sulfur
{
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

    static const float m_fatteningFactor;

    // Add your implementation here
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

    Node* CreateNode(void *clientData, Geometry::Aabb &aabb, Geometry::Aabb &origAabb, Node *parent) const;
    Node*& SelectNode(Node *nodeToInsert, Node *&left, Node *&right) const;
    void UpdateNode(Node *node) const;
    void UpdateParents(Node *node) const;

    void Insert(Node *newNode, Node *&parent);
    void StoreNodeData(std::vector<SpatialPartitionQueryData>& results, Node *node, int depth) const;
    void UpdateHeights();
    void SetHeight(Node *node) const;
    int GetHeight(Node *node) const;

    //Balancing
    int BalanceFactor(Node *node) const;
    void BalanceTree(Node *&node);
    void BalanceTreeUp(Node **node);
    void RotateLeft(Node *&node);
    void RotateRight(Node *&node);

    //Casting
    void RayCast(CastResults &results, const Geometry::Ray &ray, Node *node) const;
    void FrustumCast(const Geometry::Frustum& frustum, CastResults& results, Node *node) const;
    void CollectLeafs(CastResults &results, Node *node) const;

    void SelfQuery(QueryResults &results, Node *node) const;
    void SelfQuery(QueryResults &results, Node *node1, Node *node2) const;

    Node *m_root;
  };
}