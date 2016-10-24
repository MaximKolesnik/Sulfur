#pragma once

#include "Math\sfVector3.hpp"
#include "Math\sfQuaternion.hpp"
#include "Types\sfTypes.hpp"

namespace Sulfur
{
  namespace Physics
  {
    enum RBDynamicState
    {
      RB_Dynamic = 0,
      RB_Static,
      //RB_Kinematic
    };

    struct RigidBodyData
    {
      //Data should be initialized as soon as body is added to PhysicsWorld
      RigidBodyData(HNDL compHndl) 
        : m_compHndl(compHndl){}

      void Initialize(void);

      Vector3 m_position;
      Vector3 m_velocity;
      Vector3 m_acceleration;

      Quaternion m_orientation;

      Vector3 m_forces;

      Real m_invMass;

      RBDynamicState m_state;

      HNDL m_transformHndl;
      HNDL m_compHndl;
    };
  }
}