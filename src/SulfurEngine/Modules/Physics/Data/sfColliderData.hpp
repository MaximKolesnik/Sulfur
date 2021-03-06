/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderData.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfTypes.hpp"
#include "Math\sfVector3.hpp"
#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  class Mesh;

  namespace Physics
  {
    class ColliderGeometry;
    struct RigidBodyData;

    enum ColliderType
    {
      CT_SPHERE = 0,
      CT_CAPSULE,
      CT_MESH,
      CT_NUM_TYPES
    };

    struct ColliderData
    {
      //Data should be initialized as soon as collider is added to PhysicsWorld
      ColliderData(HNDL colHndl, ColliderType type) : m_compHndl(colHndl), m_type(type),
        m_rbData(nullptr) {}

      void Initialize(void);
      void SetMesh(const Mesh *mesh, const std::string &meshPath);
      void CalculateMass(void);

      bool m_isGhost;
      Vector3 m_offset;

      //TODO restructure this
      Vector3 m_scale;
      Real m_lineLength; //Capsule
      Real m_radius; //Sphere, Capsule

      Proxy m_proxy; //Spatial partition proxy

      ColliderType m_type;
      const ColliderGeometry *m_geometry = nullptr;

      HNDL m_transformHndl = SF_INV_HANDLE;
      HNDL m_compHndl = SF_INV_HANDLE; //Collider component hndl

      RigidBodyData *m_rbData;
    };
  }
}