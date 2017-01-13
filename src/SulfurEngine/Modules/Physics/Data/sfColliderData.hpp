/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderData.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfTypes.hpp"
#include "Math\sfVector3.hpp"
#include "Math\SpatialPartitions\sfISpatialPartition.hpp"

namespace Sulfur
{
  namespace Physics
  {
    struct RigidBodyData;

    enum ColliderType
    {
      CT_BOX = 0,
      CT_SPHERE,
      CT_CAPSULE,
      CT_NUM_TYPES
    };

    struct ColliderData
    {
      //Data should be initialized as soon as collider is added to PhysicsWorld
      ColliderData(HNDL colHndl, ColliderType type) : m_compHndl(colHndl), m_type(type),
        m_rbData(nullptr) {}

      void Initialize(void);

      bool m_isGhost;
      Vector3 m_offset;

      //TODO restructure this
      Vector3 m_scale;
      Real m_lineLength; //Capsule
      Real m_radius; //Sphere, Capsule

      Proxy m_proxy; //Spatial partition proxy
      ColliderType m_type;

      HNDL m_transformHndl = SF_INV_HANDLE;
      HNDL m_compHndl = SF_INV_HANDLE; //Collider component hndl

      RigidBodyData *m_rbData;
    };
  }
}