#include "sfNSquared.hpp"
#include "Math\Geometry\sfGeometry.hpp"

namespace Sulfur
{
  NSquaredSpatialPartition::NSquaredSpatialPartition()
  {
    m_type = SpatialPartitionTypes::NSquared;
  }

  void NSquaredSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    // Doing this lazily (and bad, but it's n-squared...).
    // Just store as the proxy what the client data is so we can look it up later.
    proxy.m_proxy = data.m_clientData;
    m_data.push_back(data.m_clientData);
  }

  void NSquaredSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    // Nothing to do here, update doesn't do anything
  }

  void NSquaredSpatialPartition::RemoveProxy(Proxy& proxy)
  {
    // Find the proxy data and remove it
    for (size_t i = 0; i < m_data.size(); ++i)
    {
      if (m_data[i] == proxy.m_proxy)
      {
        m_data[i] = m_data.back();
        m_data.pop_back();
        break;
      }
    }
  }

  void NSquaredSpatialPartition::CastRay(const Geometry::Ray& ray, CastResults& results)
  {
    // Add everything
    for (size_t i = 0; i < m_data.size(); ++i)
    {
      CastResult result;
      result.m_clientData = m_data[i];
      results.AddResult(result);
    }
  }

  void NSquaredSpatialPartition::CastFrustum(const Geometry::Frustum& frustum, CastResults& results)
  {
    // Add everything
    for (size_t i = 0; i < m_data.size(); ++i)
    {
      CastResult result;
      result.m_clientData = m_data[i];
      results.AddResult(result);
    }
  }

  void NSquaredSpatialPartition::SelfQuery(QueryResults& results)
  {
    // Add everything
    for (size_t i = 0; i < m_data.size(); ++i)
    {
      for (size_t j = i + 1; j < m_data.size(); ++j)
      {
        results.AddResult(QueryResult(m_data[i], m_data[j]));
      }
    }
  }

  void NSquaredSpatialPartition::GetDataFromProxy(const Proxy& proxy, SpatialPartitionData& data) const
  {
    data.m_clientData = proxy.m_proxy;
  }

  void NSquaredSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
  {
    for (size_t i = 0; i < m_data.size(); ++i)
    {
      SpatialPartitionQueryData data;
      data.m_clientData = m_data[i];
      results.push_back(data);
    }
  }

  //-----------------------------------------------------------------------------BoundingSphereSpatialPartition
  BoundingSphereSpatialPartition::BoundingSphereSpatialPartition()
  {
    m_type = SpatialPartitionTypes::NSquaredSphere;
  }

  void BoundingSphereSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    proxy.m_uIntProxy = (unsigned int)reinterpret_cast<uintptr_t>(&proxy);
    m_objects[(int)proxy.m_uIntProxy] = data;
  }

  void BoundingSphereSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    m_objects[(int)proxy.m_uIntProxy] = data;
  }

  void BoundingSphereSpatialPartition::RemoveProxy(Proxy& proxy)
  {
    m_objects.erase((int)proxy.m_uIntProxy);
  }

  void BoundingSphereSpatialPartition::CastRay(const Geometry::Ray& ray, CastResults& results)
  {
    for (auto &it : m_objects)
    {
      Vector3 center = it.second.m_boundingSphere.m_center;
      float radius = it.second.m_boundingSphere.m_radius;
      float t = 0;

      if (Geometry::RaySphere(ray.m_start, ray.m_direction, center, radius, t))
        results.AddResult(CastResult(it.second.m_clientData, t));
    }
  }

  void BoundingSphereSpatialPartition::CastFrustum(const Geometry::Frustum& frustum, CastResults& results)
  {
    for (auto &it : m_objects)
    {
      Vector3 center = it.second.m_boundingSphere.m_center;
      float radius = it.second.m_boundingSphere.m_radius;
      size_t lastAxis = 0;

      if (Geometry::FrustumSphere(&frustum.m_planes->m_data, center, radius, lastAxis) 
        != Geometry::IntersectionType::Outside)
        results.AddResult(CastResult(it.second.m_clientData));
    }
  }

  void BoundingSphereSpatialPartition::SelfQuery(QueryResults& results)
  {
    for (auto &it1 = m_objects.begin(); it1 != m_objects.end(); ++it1)
    {
      Vector3 center1 = it1->second.m_boundingSphere.m_center;
      float radius1 = it1->second.m_boundingSphere.m_radius;

      for (auto &it2 = std::next(it1); it2 != m_objects.end(); ++it2)
      {
        Vector3 center2 = it2->second.m_boundingSphere.m_center;
        float radius2 = it2->second.m_boundingSphere.m_radius;

        if (Geometry::SphereSphere(center1, radius1, center2, radius2))
          results.AddResult(QueryResult(it1->second.m_clientData, it2->second.m_clientData));
      }
    }
  }

  void BoundingSphereSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
  {
    for (auto &it : m_objects)
    {
      SpatialPartitionQueryData item;
      item.m_clientData = it.second.m_clientData;
      item.m_boundingSphere = it.second.m_boundingSphere;

      results.push_back(item);
    }
  }

  //-----------------------------------------------------------------------------BoundingAabbSpatialPartition
  BoundingAabbSpatialPartition::BoundingAabbSpatialPartition()
  {
    m_type = SpatialPartitionTypes::NSquaredAabb;
  }

  void BoundingAabbSpatialPartition::CreateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    proxy.m_uIntProxy = (unsigned int)reinterpret_cast<uintptr_t>(&proxy);
    m_objects[(int)proxy.m_uIntProxy] = data;
  }

  void BoundingAabbSpatialPartition::UpdateProxy(Proxy& proxy, SpatialPartitionData& data)
  {
    m_objects[(int)proxy.m_uIntProxy] = data;
  }

  void BoundingAabbSpatialPartition::RemoveProxy(Proxy& proxy)
  {
    m_objects.erase((int)proxy.m_uIntProxy);
  }

  void BoundingAabbSpatialPartition::CastRay(const Geometry::Ray& ray, CastResults& results)
  {
    for (auto &it : m_objects)
    {
      Vector3 &max = it.second.m_aabb.m_max;
      Vector3 &min = it.second.m_aabb.m_min;
      float t = 0;

      if (Geometry::RayAabb(ray.m_start, ray.m_direction, min, max, t))
        results.AddResult(CastResult(it.second.m_clientData, t));
    }
  }

  void BoundingAabbSpatialPartition::CastFrustum(const Geometry::Frustum& frustum, CastResults& results)
  {
    for (auto &it : m_objects)
    {
      Vector3 &max = it.second.m_aabb.m_max;
      Vector3 &min = it.second.m_aabb.m_min;
      size_t lastAxis = 0;

      if (Geometry::FrustumAabb(&frustum.m_planes->m_data, min, max, lastAxis) 
        != Geometry::IntersectionType::Outside)
        results.AddResult(CastResult(it.second.m_clientData));
    }
  }

  void BoundingAabbSpatialPartition::SelfQuery(QueryResults& results)
  {
    for (auto &it1 = m_objects.begin(); it1 != m_objects.end(); ++it1)
    {
      Vector3 &max1 = it1->second.m_aabb.m_max;
      Vector3 &min1 = it1->second.m_aabb.m_min;

      for (auto &it2 = std::next(it1); it2 != m_objects.end(); ++it2)
      {
        Vector3 &max2 = it2->second.m_aabb.m_max;
        Vector3 &min2 = it2->second.m_aabb.m_min;

        if (Geometry::AabbAabb(min1, max1, min2, max2))
          results.AddResult(QueryResult(it1->second.m_clientData, it2->second.m_clientData));
      }
    }
  }

  void BoundingAabbSpatialPartition::FilloutData(std::vector<SpatialPartitionQueryData>& results) const
  {
    for (auto &it : m_objects)
    {
      SpatialPartitionQueryData item;
      item.m_clientData = it.second.m_clientData;
      item.m_aabb = it.second.m_aabb;

      results.push_back(item);
    }
  }
}