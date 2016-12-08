/******************************************************************************/
/*!
\par     Sulfur
\file    sfISpatialPartition.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfISpatialPartition.hpp"

namespace Sulfur
{
  Proxy::Proxy()
  {
    m_proxy = NULL;
  }

  SpatialPartitionData::SpatialPartitionData()
  {
    m_clientData = NULL;
  }

  SpatialPartitionData::SpatialPartitionData(void* clientData, const Geometry::Aabb& aabb)
  {
    m_clientData = clientData;
    m_aabb = aabb;
  }

  SpatialPartitionData::SpatialPartitionData(void* clientData, const Geometry::Sphere& sphere)
  {
    m_clientData = clientData;
    m_boundingSphere = sphere;
  }

  bool SpatialPartitionData::operator<(const SpatialPartitionData& rhs) const
  {
    return m_clientData < rhs.m_clientData;
  }

  SpatialPartitionQueryData::SpatialPartitionQueryData()
  {
    m_depth = 0;
  }

  CastResult::CastResult()
  {
    m_time = -1;
    m_clientData = nullptr;
  }

  CastResult::CastResult(void* clientData)
  {
    m_clientData = clientData;
    m_time = -1;
  }

  CastResult::CastResult(void* clientData, Real time)
  {
    m_clientData = clientData;
    m_time = time;
  }

  bool CastResult::operator<(const CastResult& rhs) const
  {
    // Sort first by time and then resolve conflicts by the client data
    if (m_time == rhs.m_time)
      return m_clientData < rhs.m_clientData;
    return m_time < rhs.m_time;
  }

  void CastResults::AddResult(const CastResult& result)
  {
    m_results.push_back(result);

    // As long as the result before has a larger T value then this new item should be before it
    size_t index = m_results.size() - 1;
    while (index > 0 && m_results[index - 1].m_time > result.m_time)
    {
      m_results[index] = m_results[index - 1];
      --index;
    }

    m_results[index] = result;
  }

  QueryResult::QueryResult(void* clientData0, void* clientData1)
  {
    // Verify a consistent order for pairs, mainly for unit tests
    if (clientData0 < clientData1)
    {
      m_clientData0 = clientData0;
      m_clientData1 = clientData1;
    }
    else
    {
      m_clientData0 = clientData1;
      m_clientData1 = clientData0;
    }
  }

  bool QueryResult::operator==(const QueryResult& rhs) const
  {
    return m_clientData0 == rhs.m_clientData0 && m_clientData1 == rhs.m_clientData1;
  }

  bool QueryResult::operator<(const QueryResult& rhs) const
  {
    // Lexicographic sort by the Data0 then Data1
    if (m_clientData0 == rhs.m_clientData0)
      return m_clientData1 < rhs.m_clientData1;
    return m_clientData0 < rhs.m_clientData0;
  }

  void QueryResults::AddResult(const QueryResult& result)
  {
    m_results.push_back(result);
  }
}