/******************************************************************************/
/*!
\par     Sulfur
\file    sfNSquared.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "sfISpatialPartition.hpp"

namespace Sulfur
{
  class NSquaredSpatialPartition : public ISpatialPartition
  {
  public:
    NSquaredSpatialPartition();

    void CreateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void RemoveProxy(Proxy& proxy) override;

    void CastRay(const Geometry::Ray& ray, CastResults& results) override;
    void CastFrustum(const Geometry::Frustum& frustum, CastResults& results) override;

    void SelfQuery(QueryResults& results) override;

    void GetDataFromProxy(const Proxy& proxy, SpatialPartitionData& data) const override;
    void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;

    std::vector<void*> m_data;
  };

  class BoundingSphereSpatialPartition : public ISpatialPartition
  {
  public:
    BoundingSphereSpatialPartition();

    void CreateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void RemoveProxy(Proxy& proxy) override;

    void CastRay(const Geometry::Ray& ray, CastResults& results) override;
    void CastFrustum(const Geometry::Frustum& frustum, CastResults& results) override;

    void SelfQuery(QueryResults& results) override;

    void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;

  private:
    std::unordered_map<int, SpatialPartitionData> m_objects;
  };

  class BoundingAabbSpatialPartition : public ISpatialPartition
  {
  public:
    BoundingAabbSpatialPartition();

    void CreateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) override;
    void RemoveProxy(Proxy& proxy) override;

    void CastRay(const Geometry::Ray& ray, CastResults& results) override;
    void CastFrustum(const Geometry::Frustum& frustum, CastResults& results) override;

    void SelfQuery(QueryResults& results) override;

    void FilloutData(std::vector<SpatialPartitionQueryData>& results) const override;

  private:
    std::unordered_map<int, SpatialPartitionData> m_objects;
  };
}