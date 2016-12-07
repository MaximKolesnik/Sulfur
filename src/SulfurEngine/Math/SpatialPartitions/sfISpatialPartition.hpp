/******************************************************************************/
/*!
\par     Sulfur
\file    sfISpatialPartition.hpp
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
  // This class is used for uniquely identifying (and quick finding) of any object
  // in a spatial partition. When an object is inserted into the spatial partition
  // this structure should be filled out with whatever information the spatial
  // partitions needs.
  class Proxy
  {
  public:
    Proxy();

    // Most spatial partitions can get away with a pointer or an integer.
    union
    {
      void* m_proxy;
      UINT64 m_uIntProxy;
    };
  };

  // Data that must be passed to a spatial partition to insert/update an object.
  class SpatialPartitionData
  {
  public:
    SpatialPartitionData();
    SpatialPartitionData(void* clientData, const Geometry::Aabb& aabb);
    SpatialPartitionData(void* clientData, const Geometry::Sphere& sphere);

    // Comparison function used for unit testing
    bool operator<(const SpatialPartitionData& rhs) const;

    // Some data that the user wants to get back when querying the spatial partition.
    // This project uses the model component's pointer for broadphases.
    void* m_clientData;

    // The bounding aabb of the client data.
    Geometry::Aabb m_aabb;
    // The bounding sphere of the client data.
    Geometry::Sphere m_boundingSphere;
  };

  // Data returned from a spatial partition's FilloutData. Used to specify data in the
  // spatial partition and a depth (mainly for trees) to verify the structure.
  class SpatialPartitionQueryData : public SpatialPartitionData
  {
  public:
    SpatialPartitionQueryData();

    // The depth of this item in the spatial partition (tree depth, etc..). If this doesn't make sense just set to 0.
    int m_depth;
  };

  // The results from a cast through a spatial partition.
  class CastResult
  {
  public:
    CastResult();
    explicit CastResult(void* clientData);
    CastResult(void* clientData, Real time);

    // Comparison function used for unit testing
    bool operator<(const CastResult& rhs) const;

    // What time the object was hit in a raycast. In a frustum cast this left alone.
    Real m_time;
    // The object that was hit from the cast (so the user can check the original object).
    void* m_clientData;
  };

  // A collection of cast results so that a spatial partition can return multiple hit objects.
  class CastResults
  {
  public:
    // Adds the result and sorts it based upon t so that all
    // results are in proper lower t first order.
    void AddResult(const CastResult& result);

    typedef std::vector<CastResult> Results;
    Results m_results;
  };

  // A result from self query (a pair of objects)
  class QueryResult
  {
  public:
    QueryResult(void* clientData0, void* clientData1);

    // Comparison functions used for unit testing
    bool operator==(const QueryResult& rhs) const;
    bool operator<(const QueryResult& rhs) const;

    void* m_clientData0;
    void* m_clientData1;
  };

  // A collection of query results. These are not sorted in any specific order.
  class QueryResults
  {
  public:
    void AddResult(const QueryResult& result);

    typedef std::vector<QueryResult> Results;
    Results m_results;
  };

  namespace SpatialPartitionTypes
  {
    enum Types { NSquared, NSquaredSphere, NSquaredAabb, AabbTree, Unknown };
  }

  //-----------------------------------------------------------------------------SpatialPartition
  class ISpatialPartition
  {
  public:
    ISpatialPartition() { m_type = SpatialPartitionTypes::Unknown; }
    virtual ~ISpatialPartition() {};

    // Insert the given data into the spatial partition and updates the Proxy to
    // be whatever data is needed to efficiently find this object for subsequent updates/removals
    virtual void CreateProxy(Proxy& proxy, SpatialPartitionData& data) = 0;
    // Update the object represented by the proxy with the new data. This happens when an object moves.
    virtual void UpdateProxy(Proxy& proxy, SpatialPartitionData& data) = 0;
    // Remove the object represented by the proxy from this spatial partition.
    virtual void RemoveProxy(Proxy& proxy) = 0;

    virtual void BatchBuild() {};

    // Finds out what objects in the spatial partition are hit by the given ray.
    // Inserts all hits into the cast results (which sorts them by t automatically).
    virtual void CastRay(const Geometry::Ray& ray, CastResults& results) = 0;
    // Finds out what objects hit the frustum. This test is expected to be a
    // bit loose (as accurate frustum tests can be a bit expensive).
    // Also the CastResult's time should be set to 0.
    virtual void CastFrustum(const Geometry::Frustum& frustum, CastResults& results) = 0;

    // Returns pairs of all objects that are overlapping in this spatial partition.
    // This represents what physics might do to determine overlapping pairs.
    virtual void SelfQuery(QueryResults& results) = 0;

    // Debug Interface
    virtual void GetDataFromProxy(const Proxy& proxy, SpatialPartitionData& data) const {};
    // Fill out all contained data (whichever is relevant between sphere and aabb). If this is a tree then it should fill out the data in a pre-order depth first traversal.
    virtual void FilloutData(std::vector<SpatialPartitionQueryData>& results) const {};

    // What kind of spatial partition this is. Used for anttweakbar binding.
    SpatialPartitionTypes::Types m_type;
  };
}