#pragma once

#include "Types\sfIEntity.hpp"
#include "Math\sfQuaternion.hpp"
#include "Math\sfVector3.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(RigidBody, IEntity)
  public:
    RigidBody(void);
    virtual ~RigidBody(void);

    virtual void Initialize(void) override;
    virtual RigidBody* Clone(void) const override;
    virtual void Update(void) override;
    virtual void DrawDebug(DebugDraw *draw) const override;

    Physics::RBDynamicState GetDynamicState(void) const { return m_dynamicState; }

  private:
    SF_PRIVATE_PROPERTY(Vector3, velocity, Velocity, "Velocity of the object");
    Physics::RBDynamicState m_dynamicState;
    SF_PRIVATE_PROPERTY_READ_ONLY(Real, invMass, InverseMass, "Inverse Mass of the object");


  };
}