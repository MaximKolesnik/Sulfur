#include "sfAabbTree.hpp"
#include "Math\Geometry\sfShapes.hpp"

#undef small

namespace Sulfur
{
  const float DynamicAabbTree::m_fatteningFactor = 1.1f;

  DynamicAabbTree::DynamicAabbTree() : m_root(nullptr)
  {
    m_type = SpatialPartitionTypes::AabbTree;
  }

  DynamicAabbTree::~DynamicAabbTree()
  {
  }

  void DynamicAabbTree::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    if (!m_root)
    {
      m_root = CreateNode(data.m_clientData, data.m_aabb, data.m_aabb, nullptr);
      UpdateNode(m_root);
      proxy.m_proxy = m_root;
    }
    else
    {
      Node *newNode = CreateNode(data.m_clientData, data.m_aabb, data.m_aabb, nullptr);
      UpdateNode(newNode);
      Insert(newNode, m_root);
      UpdateHeights();
      proxy.m_proxy = newNode;
    }
  }

  void DynamicAabbTree::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    Node *node = reinterpret_cast<Node*>(proxy.m_proxy);

    if (!node->m_parent)
    {
      node->m_aabb = data.m_aabb;
      node->m_origAbbb = data.m_aabb;
      UpdateNode(node);
      return;
    }
    else
    {
      if (node->m_parent->m_aabb.Contains(data.m_aabb))
      {
        node->m_aabb = data.m_aabb;
        node->m_origAbbb = data.m_aabb;
        UpdateNode(node);
        return;
      }
      else
      {
        RemoveProxy(proxy);
        CreateProxy(proxy, data);
      }
    }
  }

  void DynamicAabbTree::RemoveProxy(Proxy& proxy)
  {
    Node *node = reinterpret_cast<Node*>(proxy.m_proxy);

    Node *parent = node->m_parent;
    if (parent)
    {
      Node *sibling = parent->m_left == node ? parent->m_right : parent->m_left;
      Node *grandParent = parent->m_parent;
      if (!grandParent)
      {
        m_root = sibling;
        sibling->m_parent = nullptr;
      }
      else
      {
        sibling->m_parent = grandParent;
        if (parent == parent->m_parent->m_left)
          grandParent->m_left = sibling;
        else
          grandParent->m_right = sibling;
      }
      delete parent;
      parent = nullptr;

      UpdateHeights();
      BalanceTreeUp(&sibling);
      UpdateNode(sibling);
    }
    else
      m_root = nullptr;

    delete node;

  }

  void DynamicAabbTree::CastRay(const Geometry::Ray& ray, CastResults& results)
  {
    RayCast(results, ray, m_root);
  }

  void DynamicAabbTree::CastFrustum(const Geometry::Frustum& frustum, CastResults& results)
  {
    FrustumCast(frustum, results, m_root);
  }

  void DynamicAabbTree::SelfQuery(QueryResults& results)
  {
    SelfQuery(results, m_root);
  }

  void DynamicAabbTree::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
  {
    StoreNodeData(results, m_root, 0);
  }

  DynamicAabbTree::Node* DynamicAabbTree::CreateNode(void *clientData, Geometry::Aabb &aabb,
    Geometry::Aabb &origAabb, Node *parent) const
  {
    Node *newNode = new Node();

    newNode->m_clientData = clientData;
    newNode->m_aabb = aabb;
    newNode->m_origAbbb = origAabb;
    newNode->m_left = newNode->m_right = nullptr;
    newNode->m_parent = parent;
    newNode->m_height = 0;
    newNode->m_lastAxis = 0;

    return newNode;
  }

  void DynamicAabbTree::Node::SetChildren(Node *left, Node *right)
  {
    this->m_left = left;
    this->m_right = right;
  }

  DynamicAabbTree::Node*& DynamicAabbTree::SelectNode(Node *nodeToInsert, Node *&left, Node *&right) const
  {
    float leftDiff = Geometry::Aabb::Combine(nodeToInsert->m_aabb, left->m_aabb).GetSurfaceArea() - left->m_aabb.GetSurfaceArea();
    float rightDiff = Geometry::Aabb::Combine(nodeToInsert->m_aabb, right->m_aabb).GetSurfaceArea() - right->m_aabb.GetSurfaceArea();

    if (leftDiff < rightDiff)
      return left;
    else
      return right;
  }

  void DynamicAabbTree::UpdateNode(Node *node) const
  {
    if (node->IsLeaf())
    {
      Vector3 newHalfSize = node->m_origAbbb.GetHalfSize() * m_fatteningFactor;
      node->m_aabb = Geometry::Aabb::BuildFromCenterAndHalfExtents(node->m_origAbbb.GetCenter(), newHalfSize);
    }
    else
      node->m_aabb = Geometry::Aabb::Combine(node->m_left->m_aabb, node->m_right->m_aabb);
  }

  void DynamicAabbTree::UpdateParents(Node *node) const
  {
    if (node->m_parent)
    {
      UpdateNode(node->m_parent);
      UpdateParents(node->m_parent);
    }
  }

  void DynamicAabbTree::Insert(Node *newNode, Node *&parent)
  {
    if (!parent->IsLeaf())
    {
      Node *&selection = SelectNode(newNode, parent->m_left, parent->m_right);
      Insert(newNode, selection);
    }
    else
    {
      Node *branch = CreateNode(nullptr, parent->m_aabb, parent->m_aabb, parent->m_parent);
      branch->SetChildren(parent, newNode);

      branch->m_left->m_parent = branch;
      branch->m_right->m_parent = branch;

      parent = branch;
    }

    SetHeight(m_root);
    BalanceTree(parent);
    UpdateNode(parent);
    UpdateParents(parent);
  }

  void DynamicAabbTree::StoreNodeData(std::vector<SpatialPartitionQueryData>& results,
    Node *node, int depth) const
  {
    if (!node)
      return;

    SpatialPartitionQueryData data;
    data.m_aabb = node->m_aabb;
    data.m_clientData = node->m_clientData;
    data.m_depth = depth;

    results.push_back(data);

    StoreNodeData(results, node->m_left, depth + 1);
    StoreNodeData(results, node->m_right, depth + 1);
  }

  void DynamicAabbTree::UpdateHeights()
  {
    SetHeight(m_root);
  }

  void DynamicAabbTree::SetHeight(Node *node) const
  {
    if (node->IsLeaf())
      node->m_height = 0;
    else
    {
      SetHeight(node->m_left);
      SetHeight(node->m_right);
      int leftH = node->m_left->m_height;
      int rightH = node->m_right->m_height;
      node->m_height = (leftH > rightH ? leftH : rightH) + 1;
    }
  }

  int DynamicAabbTree::GetHeight(Node *node) const
  {
    return node ? node->m_height : 0;
  }

  int DynamicAabbTree::BalanceFactor(Node *node) const
  {
    if (!node)
      return 0;

    return GetHeight(node->m_right) - GetHeight(node->m_left);
  }

  void DynamicAabbTree::BalanceTree(Node *&node)
  {
    if (BalanceFactor(node) >= 2)
      RotateLeft(node);
    if (BalanceFactor(node) <= -2)
      RotateRight(node);
  }

  void DynamicAabbTree::BalanceTreeUp(Node **node)
  {
    if (!*node)
      return;

    BalanceTree((*node));

    BalanceTreeUp(&(*node)->m_parent);
  }

  void DynamicAabbTree::RotateLeft(Node *&node)
  {
    Node *nodeParent = node->m_parent;
    Node *newRoot = node->m_right;
    Node *small;

    if (BalanceFactor(newRoot) > 0)
    {
      small = newRoot->m_left;

      newRoot->m_left = node;
      newRoot->m_parent = node->m_parent;
      node->m_parent = newRoot;
      node->m_right = small;

      small->m_parent = node;
    }
    else
    {
      small = newRoot->m_right;

      newRoot->m_right = node;
      newRoot->m_parent = node->m_parent;
      node->m_parent = newRoot;
      node->m_right = small;

      small->m_parent = node;
    }

    if (node == m_root)
      m_root = newRoot;
    else
    {
      if (nodeParent->m_right == node)
        nodeParent->m_right = newRoot;
      else
        nodeParent->m_left = newRoot;
    }

    SetHeight(newRoot);
    SetHeight(node);
    UpdateNode(newRoot->m_left);
    UpdateNode(newRoot->m_right);
    UpdateNode(newRoot);
  }

  void DynamicAabbTree::RotateRight(Node *&node)
  {
    Node *nodeParent = node->m_parent;
    Node *newRoot = node->m_left;
    Node *small;

    if (BalanceFactor(newRoot) > 0)
    {
      small = newRoot->m_left;
      newRoot->m_left = node;
      newRoot->m_parent = node->m_parent;
      node->m_parent = newRoot;
      node->m_left = small;

      small->m_parent = node;
    }
    else
    {

      small = newRoot->m_right;
      newRoot->m_right = node;
      newRoot->m_parent = node->m_parent;
      node->m_parent = newRoot;
      node->m_left = small;

      small->m_parent = node;
    }

    if (node == m_root)
      m_root = newRoot;
    else
    {
      if (nodeParent->m_right == node)
        nodeParent->m_right = newRoot;
      else
        nodeParent->m_left = newRoot;
    }
    SetHeight(newRoot);
    SetHeight(node);
    UpdateNode(newRoot->m_left);
    UpdateNode(newRoot->m_right);
    UpdateNode(newRoot);
  }

  void DynamicAabbTree::RayCast(CastResults &results, const Geometry::Ray &ray, Node *node) const
  {
    if (!node)
      return;

    float t;
    if (Geometry::RayAabb(ray.mStart, ray.mDirection, node->m_aabb.mMin, node->m_aabb.mMax, t))
    {

      if (node->IsLeaf())
        results.AddResult(CastResult(node->m_clientData, t));
      else
      {
        RayCast(results, ray, node->m_left);
        RayCast(results, ray, node->m_right);
      }
    }
  }

  void DynamicAabbTree::FrustumCast(const Geometry::Frustum& frustum, CastResults& results,
    Node *node) const
  {
    if (!node)
      return;

    Geometry::IntersectionType::Type result =
      Geometry::FrustumAabb(&frustum.mPlanes->mData, node->m_aabb.mMin,
      node->m_aabb.mMax, node->m_lastAxis);

    if (result == Geometry::IntersectionType::Inside)
    {
      CollectLeafs(results, node);
      return;
    }

    if (result != Geometry::IntersectionType::Outside)
    {
      if (node->IsLeaf())
        results.AddResult(CastResult(node->m_clientData));
      else
      {
        FrustumCast(frustum, results, node->m_left);
        FrustumCast(frustum, results, node->m_right);
      }
    }
  }

  void DynamicAabbTree::CollectLeafs(CastResults &results, Node *node) const
  {
    if (!node)
      return;

    if (node->IsLeaf())
      results.AddResult(CastResult(node->m_clientData));
    else
    {
      CollectLeafs(results, node->m_left);
      CollectLeafs(results, node->m_right);
    }
  }

  void DynamicAabbTree::SelfQuery(QueryResults &results, Node *node) const
  {
    if (!node || node->IsLeaf())
      return;

    SelfQuery(results, node->m_left, node->m_right);
    SelfQuery(results, node->m_left);
    SelfQuery(results, node->m_right);
  }

  void DynamicAabbTree::SelfQuery(QueryResults &results, Node *node1, Node *node2) const
  {
    if (!node1 || !node2)
      return;

    if (Geometry::AabbAabb(node1->m_aabb.mMin, node1->m_aabb.mMax, node2->m_aabb.mMin, node2->m_aabb.mMax))
    {
      if (node1->IsLeaf() && node2->IsLeaf())
      {
        results.AddResult(QueryResult(node1->m_clientData, node2->m_clientData));
        return;
      }
      else if (node1->IsLeaf())
      {
        SelfQuery(results, node1, node2->m_left);
        SelfQuery(results, node1, node2->m_right);
      }
      else if (node2->IsLeaf())
      {
        SelfQuery(results, node2, node1->m_left);
        SelfQuery(results, node2, node1->m_right);
      }
      else if (node2->m_aabb.GetVolume() > node1->m_aabb.GetVolume())
      {
        SelfQuery(results, node1, node2->m_left);
        SelfQuery(results, node1, node2->m_right);
      }
      else
      {
        SelfQuery(results, node2, node1->m_left);
        SelfQuery(results, node2, node1->m_right);
      }
    }
  }
}