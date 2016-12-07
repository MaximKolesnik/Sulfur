/******************************************************************************/
/*!
\par     Sulfur
\file    sfPhysicsWorld.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>

#include "Types\sfSingleton.hpp"
#include "Types\sfTypes.hpp"
#include "Math\sfVector3.hpp"
#include "Managers\TaskManager\sfTaskRegistry.hpp"
#include "Data\sfColliderData.hpp"

namespace Sulfur
{
  class DynamicAabbTree;

  SF_DECLARE_TASK(SyncData);
  SF_DECLARE_TASK(IntegrateBodies);
  SF_DECLARE_TASK(BroadPhase);
  SF_DECLARE_TASK(NarrowPhase);
  SF_DECLARE_TASK(PostAndCleanup);
  
  namespace Physics
  {
    struct RigidBodyData;
    class BroadPhase;
    class NarrowPhase;
    class PhysicsWorld
    {
      SF_SINGLETON(PhysicsWorld);
    public:
      static const Vector3 c_gravity;
      static const Real c_allowedPenetration;
      static const Real c_biasFactor;

      void Initialize(void);

      void AddRigidBody(HNDL owner, HNDL rbHndl);
      void RemoveRigidBody(HNDL owner, HNDL rbHndl);

      void AddCollider(HNDL owner, HNDL colHndl, ColliderType type);
      void RemoveCollider(HNDL owner, HNDL colHndl);

      RigidBodyData* GetRigidBodyData(HNDL owner);
      ColliderData* GetColliderData(HNDL owner);

      typedef std::unordered_map<HNDL, RigidBodyData*> RigidBodyList;
      typedef std::unordered_map<HNDL, ColliderData*> ColliderList;

      //Debug
      void DebugDrawColliders(void) const;
      void DebugDrawAABB(void) const;
    private:
      SF_FRIEND_TASK(Sulfur::IntegrateBodies);
      SF_FRIEND_TASK(Sulfur::SyncData);
      SF_FRIEND_TASK(Sulfur::PostAndCleanup);
      SF_FRIEND_TASK(Sulfur::BroadPhase);
      SF_FRIEND_TASK(Sulfur::NarrowPhase);

      RigidBodyData* _CreateDummyRigidBodyData(HNDL owner) const;

      QueryResults m_possiblePairs;

      RigidBodyList m_rigidBodies;
      ColliderList m_colliders;
      BroadPhase *m_broadPhase;
    };
  }
}