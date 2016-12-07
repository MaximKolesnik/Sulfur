#pragma once

#include "Math\sfVector3.hpp"
#include "Math\sfQuaternion.hpp"
#include "Types\sfTypes.hpp"
#include "Math\sfMatrix3.hpp"
#include "Reflection\sfReflection.hpp"

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

      Vector3 m_position = Vector3::c_zero;
      Vector3 m_velocity = Vector3::c_zero;
      Vector3 m_angularVelocity = Vector3::c_zero;
      Vector3 m_acceleration = Vector3::c_zero;

      Quaternion m_orientation;

      Vector3 m_forces = Vector3::c_zero;

      Real m_invMass = 0;
      
      Matrix3 m_inertia = Matrix3::c_identity;
      Matrix3 m_invInertia = Matrix3::c_identity;

      RBDynamicState m_state = RB_Static;

      HNDL m_transformHndl = SF_INV_HANDLE;
      HNDL m_compHndl = SF_INV_HANDLE;
    };
  }

  SF_REGISTER_ENUM_TYPE(Physics::RBDynamicState, "RB_Dynamic", "RB_Static");
  SF_BASE_TYPE_SPECIALIZATION(Physics::RBDynamicState);
}