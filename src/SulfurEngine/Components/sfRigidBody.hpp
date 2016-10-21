#pragma once

#include "Types\sfIEntity.hpp"
#include "Math\sfQuaternion.hpp"
#include "Math\sfVector3.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(RigidBody, IEntity)
  public:
    RigidBody(void);
    virtual ~RigidBody(void);
  private:
    Vector3 m_position;
    Vector3 m_velocity;
    Vector3 m_acceleration;

    Quaternion m_orientation;

    Vector3 m_forces;

    Real m_mass;

    bool m_isStatic;
  };
}