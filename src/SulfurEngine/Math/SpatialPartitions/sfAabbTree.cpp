#include "sfAabbTree.hpp"
#include "Math\Geometry\sfShapes.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"
#include "Math\sfVector4.hpp"

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
      m_root = _CreateNode(data.m_clientData, data.m_aabb, data.m_aabb, nullptr);
      _UpdateNode(m_root);
      proxy.m_proxy = m_root;
    }
    else
    {
      Node *newNode = _CreateNode(data.m_clientData, data.m_aabb, data.m_aabb, nullptr);
      _UpdateNode(newNode);
      _Insert(newNode, m_root);
      _UpdateHeights();
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
      _UpdateNode(node);
      return;
    }
    else
    {
      if (node->m_parent->m_aabb.Contains(data.m_aabb))
      {
        node->m_aabb = data.m_aabb;
        node->m_origAbbb = data.m_aabb;
        _UpdateNode(node);
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

      _UpdateHeights();
      _BalanceTreeUp(&sibling);
      _UpdateNode(sibling);
    }
    else
      m_root = nullptr;

    delete node;

  }

  void DynamicAabbTree::CastRay(const Geometry::Ray& ray, CastResults& results)
  {
    _RayCast(results, ray, m_root);
  }

  void DynamicAabbTree::CastFrustum(const Geometry::Frustum& frustum, CastResults& results)
  {
    _FrustumCast(frustum, results, m_root);
  }

  void DynamicAabbTree::SelfQuery(QueryResults& results)
  {
    _SelfQuery(results, m_root);
  }

  void DynamicAabbTree::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
  {
    _StoreNodeData(results, m_root, 0);
  }

  DynamicAabbTree::Node* DynamicAabbTree::_CreateNode(void *clientData, Geometry::Aabb &aabb,
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

  DynamicAabbTree::Node*& DynamicAabbTree::_SelectNode(Node *nodeToInsert, Node *&left, Node *&right) const
  {
    float leftDiff = Geometry::Aabb::Combine(nodeToInsert->m_aabb, left->m_aabb).GetSurfaceArea() - left->m_aabb.GetSurfaceArea();
    float rightDiff = Geometry::Aabb::Combine(nodeToInsert->m_aabb, right->m_aabb).GetSurfaceArea() - right->m_aabb.GetSurfaceArea();

    if (leftDiff < rightDiff)
      return left;
    else
      return right;
  }

  void DynamicAabbTree::_UpdateNode(Node *node) const
  {
    if (node->IsLeaf())
    {
      Vector3 newHalfSize = node->m_origAbbb.GetHalfSize() * m_fatteningFactor;
      node->m_aabb = Geometry::Aabb::BuildFromCenterAndHalfExtents(node->m_origAbbb.GetCenter(), newHalfSize);
    }
    else
      node->m_aabb = Geometry::Aabb::Combine(node->m_left->m_aabb, node->m_right->m_aabb);
  }

  void DynamicAabbTree::_UpdateParents(Node *node) const
  {
    if (node->m_parent)
    {
      _UpdateNode(node->m_parent);
      _UpdateParents(node->m_parent);
    }
  }

  void DynamicAabbTree::_Insert(Node *newNode, Node *&parent)
  {
    if (!parent->IsLeaf())
    {
      Node *&selection = _SelectNode(newNode, parent->m_left, parent->m_right);
      _Insert(newNode, selection);
    }
    else
    {
      Node *branch = _CreateNode(nullptr, parent->m_aabb, parent->m_aabb, parent->m_parent);
      branch->SetChildren(parent, newNode);

      branch->m_left->m_parent = branch;
      branch->m_right->m_parent = branch;

      parent = branch;
    }

    _SetHeight(m_root);
    _BalanceTree(parent);
    _UpdateNode(parent);
    _UpdateParents(parent);
  }

  void DynamicAabbTree::_StoreNodeData(std::vector<SpatialPartitionQueryData>& results,
    Node *node, int depth) const
  {
    if (!node)
      return;

    SpatialPartitionQueryData data;
    data.m_aabb = node->m_aabb;
    data.m_clientData = node->m_clientData;
    data.m_depth = depth;

    results.push_back(data);

    _StoreNodeData(results, node->m_left, depth + 1);
    _StoreNodeData(results, node->m_right, depth + 1);
  }

  void DynamicAabbTree::_UpdateHeights()
  {
    _SetHeight(m_root);
  }

  void DynamicAabbTree::_SetHeight(Node *node) const
  {
    if (node->IsLeaf())
      node->m_height = 0;
    else
    {
      _SetHeight(node->m_left);
      _SetHeight(node->m_right);
      int leftH = node->m_left->m_height;
      int rightH = node->m_right->m_height;
      node->m_height = (leftH > rightH ? leftH : rightH) + 1;
    }
  }

  int DynamicAabbTree::_GetHeight(Node *node) const
  {
    return node ? node->m_height : 0;
  }

  int DynamicAabbTree::_BalanceFactor(Node *node) const
  {
    if (!node)
      return 0;

    return _GetHeight(node->m_right) - _GetHeight(node->m_left);
  }

  void DynamicAabbTree::_BalanceTree(Node *&node)
  {
    if (_BalanceFactor(node) >= 2)
      _RotateLeft(node);
    if (_BalanceFactor(node) <= -2)
      _RotateRight(node);
  }

  void DynamicAabbTree::_BalanceTreeUp(Node **node)
  {
    if (!*node)
      return;

    _BalanceTree((*node));

    _BalanceTreeUp(&(*node)->m_parent);
  }

  void DynamicAabbTree::_RotateLeft(Node *&node)
  {
    Node *nodeParent = node->m_parent;
    Node *newRoot = node->m_right;
    Node *small;

    if (_BalanceFactor(newRoot) > 0)
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

    _SetHeight(newRoot);
    _SetHeight(node);
    _UpdateNode(newRoot->m_left);
    _UpdateNode(newRoot->m_right);
    _UpdateNode(newRoot);
  }

  void DynamicAabbTree::_RotateRight(Node *&node)
  {
    Node *nodeParent = node->m_parent;
    Node *newRoot = node->m_left;
    Node *small;

    if (_BalanceFactor(newRoot) > 0)
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
    _SetHeight(newRoot);
    _SetHeight(node);
    _UpdateNode(newRoot->m_left);
    _UpdateNode(newRoot->m_right);
    _UpdateNode(newRoot);
  }

  void DynamicAabbTree::_RayCast(CastResults &results, const Geometry::Ray &ray, Node *node) const
  {
    if (!node)
      return;

    float t;
    if (Geometry::RayAabb(ray.m_start, ray.m_direction, 
      node->m_aabb.m_min, node->m_aabb.m_max, t))
    {

      if (node->IsLeaf())
        results.AddResult(CastResult(node->m_clientData, t));
      else
      {
        _RayCast(results, ray, node->m_left);
        _RayCast(results, ray, node->m_right);
      }
    }
  }

  void DynamicAabbTree::_FrustumCast(const Geometry::Frustum& frustum, CastResults& results,
    Node *node) const
  {
    if (!node)
      return;

    Geometry::IntersectionType::Type result =
      Geometry::FrustumAabb(&frustum.m_planes->m_data, node->m_aabb.m_min,
      node->m_aabb.m_max, node->m_lastAxis);

    if (result == Geometry::IntersectionType::Inside)
    {
      _CollectLeafs(results, node);
      return;
    }

    if (result != Geometry::IntersectionType::Outside)
    {
      if (node->IsLeaf())
        results.AddResult(CastResult(node->m_clientData));
      else
      {
        _FrustumCast(frustum, results, node->m_left);
        _FrustumCast(frustum, results, node->m_right);
      }
    }
  }

  void DynamicAabbTree::_CollectLeafs(CastResults &results, Node *node) const
  {
    if (!node)
      return;

    if (node->IsLeaf())
      results.AddResult(CastResult(node->m_clientData));
    else
    {
      _CollectLeafs(results, node->m_left);
      _CollectLeafs(results, node->m_right);
    }
  }

  void DynamicAabbTree::_SelfQuery(QueryResults &results, Node *node) const
  {
    if (!node || node->IsLeaf())
      return;

    _SelfQuery(results, node->m_left, node->m_right);
    _SelfQuery(results, node->m_left);
    _SelfQuery(results, node->m_right);
  }

  void DynamicAabbTree::_SelfQuery(QueryResults &results, Node *node1, Node *node2) const
  {
    if (!node1 || !node2)
      return;

    if (Geometry::AabbAabb(node1->m_aabb.m_min, node1->m_aabb.m_max, 
      node2->m_aabb.m_min, node2->m_aabb.m_max))
    {
      if (node1->IsLeaf() && node2->IsLeaf())
      {
        results.AddResult(QueryResult(node1->m_clientData, node2->m_clientData));
        return;
      }
      else if (node1->IsLeaf())
      {
        _SelfQuery(results, node1, node2->m_left);
        _SelfQuery(results, node1, node2->m_right);
      }
      else if (node2->IsLeaf())
      {
        _SelfQuery(results, node2, node1->m_left);
        _SelfQuery(results, node2, node1->m_right);
      }
      else if (node2->m_aabb.GetVolume() > node1->m_aabb.GetVolume())
      {
        _SelfQuery(results, node1, node2->m_left);
        _SelfQuery(results, node1, node2->m_right);
      }
      else
      {
        _SelfQuery(results, node2, node1->m_left);
        _SelfQuery(results, node2, node1->m_right);
      }
    }
  }

  void DynamicAabbTree::DrawDebug(DebugDraw *draw) const
  {
    _DrawDebug(m_root, draw);
  }

  void DynamicAabbTree::_DrawDebug(Node *node, DebugDraw *draw) const
  {
    if (node == nullptr)
      return;

    if (node->m_height == 0)
    {
      Matrix4 m;
      Vector3 aabbCenter = node->m_aabb.GetCenter();
      Vector3 hs = node->m_aabb.GetHalfSize();
      //m.SetTranslation(aabbCenter[0], aabbCenter[1], aabbCenter[2]);
      //m.SetScaling(hs[0] * Real(5.0), hs[1] * Real(5.0), hs[2] * Real(5.0));
      m.SetTransformation(Quaternion(1.0, 0, 0, 0), hs, aabbCenter);
      draw->DrawBox(m, 2, 2, 2, true, Vector4(255.0, 0.0, 0.0, 0.0));
    }

    _DrawDebug(node->m_left, draw);
    _DrawDebug(node->m_right, draw);
  }
}