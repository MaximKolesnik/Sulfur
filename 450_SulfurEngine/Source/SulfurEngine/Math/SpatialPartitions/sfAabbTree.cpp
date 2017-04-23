/******************************************************************************/
/*!
\par     Sulfur
\file    sfAabbTree.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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
    Node *node = _CreateNode(data.m_clientData, nullptr);
    node->m_aabb = data.m_aabb;
    
    _Insert(node);

    proxy.m_proxy = node;
  }

  void DynamicAabbTree::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    Node *node = reinterpret_cast<Node*>(proxy.m_proxy);
    node->m_aabb = data.m_aabb;

    _RemoveNode(node);
    _Insert(node);
  }

  void DynamicAabbTree::RemoveProxy(Proxy& proxy)
  {
    Node *node = reinterpret_cast<Node*>(proxy.m_proxy);

    _RemoveNode(node);

    delete node;
    proxy.m_proxy = nullptr;
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

  DynamicAabbTree::Node* DynamicAabbTree::_CreateNode(void *clientData, Node *parent) const
  {
    Node *newNode = new Node();

    newNode->m_clientData = clientData;
    newNode->m_left = newNode->m_right = nullptr;
    newNode->m_parent = parent;
    newNode->m_height = 0;

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

  /*void DynamicAabbTree::_UpdateNode(Node *node) const
  {
    if (node->IsLeaf())
    {
      Vector3 newHalfSize = node->m_origAbbb.GetHalfSize() * m_fatteningFactor;
      node->m_aabb = Geometry::Aabb::BuildFromCenterAndHalfExtents(node->m_origAbbb.GetCenter(), newHalfSize);
    }
    else
      node->m_aabb = Geometry::Aabb::Combine(node->m_left->m_aabb, node->m_right->m_aabb);
  }*/

  /*void DynamicAabbTree::_UpdateParents(Node *node) const
  {
    if (node->m_parent)
    {
      _UpdateNode(node->m_parent);
      _UpdateParents(node->m_parent);
    }
  }*/

  void DynamicAabbTree::_Insert(Node *newNode)
  {
    if (!m_root)
    {
      m_root = newNode;
      newNode->m_parent = nullptr;
      return;
    }

    const Geometry::Aabb &newAabb = newNode->m_aabb;
    Node *current = m_root;

    while (!current->IsLeaf())
    {
      Real area = current->m_aabb.GetSurfaceArea();

      Geometry::Aabb combined;
      combined = Geometry::Aabb::Combine(current->m_aabb, newAabb);
      Real combinedArea = combined.GetSurfaceArea();

      Real cost = Real(2.0) * combinedArea;
      Real pushingCost = Real(2.0) * (combinedArea - area);

      Real costLeft;
      if (current->m_left->IsLeaf())
      {
        Geometry::Aabb aabb;
        aabb = Geometry::Aabb::Combine(newAabb, current->m_left->m_aabb);
        costLeft = aabb.GetSurfaceArea() + pushingCost;
      }
      else
      {
        Geometry::Aabb aabb;
        aabb = Geometry::Aabb::Combine(newAabb, current->m_left->m_aabb);
        Real oldArea = current->m_left->m_aabb.GetSurfaceArea();
        Real newArea = aabb.GetSurfaceArea();
        costLeft = (newArea - oldArea) + pushingCost;
      }

      Real costRight;
      if (current->m_right->IsLeaf())
      {
        Geometry::Aabb aabb;
        aabb = Geometry::Aabb::Combine(newAabb, current->m_right->m_aabb);
        costRight = aabb.GetSurfaceArea() + pushingCost;
      }
      else
      {
        Geometry::Aabb aabb;
        aabb = Geometry::Aabb::Combine(newAabb, current->m_right->m_aabb);
        Real oldArea = current->m_right->m_aabb.GetSurfaceArea();
        Real newArea = aabb.GetSurfaceArea();
        costRight = (newArea - oldArea) + pushingCost;
      }

      if (cost < costLeft && cost < costRight)
        break;

      if (costLeft < costRight)
        current = current->m_left;
      else
        current = current->m_right;
    }

    Node *sibling = current;

    Node *oldParent = sibling->m_parent;
    Node *newParent = _CreateNode(nullptr, oldParent);
    newParent->m_aabb = Geometry::Aabb::Combine(newAabb, sibling->m_aabb);
    newParent->m_height = sibling->m_height + 1;

    if (oldParent)
    {
      if (oldParent->m_left == sibling)
        oldParent->m_left = newParent;
      else
        oldParent->m_right = newParent;

      newParent->m_left = sibling;
      newParent->m_right = newNode;
      sibling->m_parent = newParent;
      newNode->m_parent = newParent;
    }
    else
    {
      newParent->m_left = sibling;
      newParent->m_right = newNode;
      sibling->m_parent = newParent;
      newNode->m_parent = newParent;
      m_root = newParent;
    }

    current = newNode->m_parent;
    while (current)
    {
      current = _BalanceNode(current);

      current->m_height = 1 + std::max(current->m_left->m_height, current->m_right->m_height);
      current->m_aabb = Geometry::Aabb::Combine(current->m_left->m_aabb, current->m_right->m_aabb);

      current = current->m_parent;
    }
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

  DynamicAabbTree::Node* DynamicAabbTree::_BalanceNode(Node *node)
  {
    if (node->IsLeaf() || node->m_height < 2)
      return node;

    Node *leftChild = node->m_left;
    Node *rightChild = node->m_right;

    int balanceFactor = 
      ((rightChild->m_right) ? rightChild->m_right->m_height : 0)
      - 
      ((leftChild->m_left) ? leftChild->m_left->m_height : 0);

    if (balanceFactor > 1)
      return _RotateLeft(rightChild);

    if (balanceFactor < -1)
      return _RotateRight(leftChild);

    return node;
  }

  DynamicAabbTree::Node* DynamicAabbTree::_RotateLeft(Node *&node)
  {
    Node *parent = node->m_parent;
    Node *sibling = parent->m_left;
    Node *leftChild = node->m_left;
    Node *rightChild = node->m_right;

    node->m_left = parent;
    node->m_parent = parent->m_parent;
    parent->m_parent = node;

    if (node->m_parent)
    {
      if (node->m_parent->m_left == parent)
        node->m_parent->m_left = parent->m_right;
      else
        node->m_parent->m_right = parent->m_right;
    }
    else
      m_root = node;

    if (leftChild->m_height > rightChild->m_height)
    {
      node->m_right = leftChild;
      parent->m_right = rightChild;
      rightChild->m_parent = parent;

      parent->m_aabb = Geometry::Aabb::Combine(sibling->m_aabb, rightChild->m_aabb);
      node->m_aabb = Geometry::Aabb::Combine(parent->m_aabb, leftChild->m_aabb);

      parent->m_height = 1 + std::max(sibling->m_height, rightChild->m_height);
      node->m_height = 1 + std::max(parent->m_height, leftChild->m_height);
    }
    else
    {
      node->m_right = rightChild;
      parent->m_right = leftChild;
      leftChild->m_parent = parent;

      parent->m_aabb = Geometry::Aabb::Combine(sibling->m_aabb, leftChild->m_aabb);
      node->m_aabb = Geometry::Aabb::Combine(parent->m_aabb, rightChild->m_aabb);

      parent->m_height = 1 + std::max(sibling->m_height, leftChild->m_height);
      node->m_height = 1 + std::max(parent->m_height, rightChild->m_height);
    }

    return node;
  }

  DynamicAabbTree::Node* DynamicAabbTree::_RotateRight(Node *&node)
  {
    Node *parent = node->m_parent;
    Node *sibling = parent->m_right;
    Node *leftChild = node->m_left;
    Node *rightChild = node->m_right;

    node->m_left = parent;
    node->m_parent = parent->m_parent;
    parent->m_parent = node;

    if (node->m_parent)
    {
      if (node->m_parent->m_left == parent)
        node->m_parent->m_left = node;
      else
        node->m_parent->m_right = node;
    }
    else
      m_root = node;

    if (leftChild->m_height > rightChild->m_height)
    {
      node->m_right = leftChild;
      parent->m_left = rightChild;
      rightChild->m_parent = parent;

      parent->m_aabb = Geometry::Aabb::Combine(sibling->m_aabb, rightChild->m_aabb);
      node->m_aabb = Geometry::Aabb::Combine(parent->m_aabb, leftChild->m_aabb);

      parent->m_height = 1 + std::max(sibling->m_height, rightChild->m_height);
      node->m_height = 1 + std::max(parent->m_height, leftChild->m_height);
    }
    else
    {
      node->m_right = rightChild;
      parent->m_left = leftChild;
      leftChild->m_parent = parent;

      parent->m_aabb = Geometry::Aabb::Combine(sibling->m_aabb, leftChild->m_aabb);
      node->m_aabb = Geometry::Aabb::Combine(parent->m_aabb, rightChild->m_aabb);

      parent->m_height = 1 + std::max(sibling->m_height, leftChild->m_height);
      node->m_height = 1 + std::max(parent->m_height, rightChild->m_height);
    }

    return node;
  }

  void DynamicAabbTree::_RemoveNode(Node *node)
  {
    if (node == m_root)
    {
      m_root = nullptr;
      return;
    }

    Node *parent = node->m_parent;
    Node *grandParent = parent->m_parent;
    Node *sibling;

    if (parent->m_left == node)
      sibling = parent->m_right;
    else
      sibling = parent->m_left;

    if (grandParent)
    {
      if (grandParent->m_left == parent)
        grandParent->m_left = sibling;
      else
        grandParent->m_right = sibling;

      sibling->m_parent = grandParent;

      delete parent;

      Node *curr = grandParent;
      while (curr)
      {
        curr = _BalanceNode(curr);

        curr->m_aabb = Geometry::Aabb::Combine(curr->m_left->m_aabb, curr->m_right->m_aabb);
        curr->m_height = 1 + std::max(curr->m_left->m_height, curr->m_right->m_height);

        curr = curr->m_parent;
      }
    }
    else
    {
      m_root = sibling;
      sibling->m_parent = nullptr;
      delete parent;
    }
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

    size_t lastAxis = 0;
    Geometry::IntersectionType::Type result =
      Geometry::FrustumAabb(&frustum.m_planes->m_data, node->m_aabb.m_min,
      node->m_aabb.m_max, lastAxis);

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