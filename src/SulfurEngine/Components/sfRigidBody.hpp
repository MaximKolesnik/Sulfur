/******************************************************************************/
/*!
\par     Sulfur
\file    sfRigidBody.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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

    const Vector3& GetVelocity(void) const;
    const Vector3& GetAngularVelocity(void) const;
    const Physics::RBDynamicState& GetDynamicState(void) const;

    void SetVelocity(const Vector3 &vel);
    void SetAngularVelocity(const Vector3 &vel);
    void SetDynamicState(const Physics::RBDynamicState &vel);
  private:
    SF_PRIVATE_PROPERTY_CUSTOM(Vector3, velocity, Velocity, "Velocity of the object",
      GetVelocity, SetVelocity);

    SF_PRIVATE_PROPERTY_CUSTOM(Vector3, angularVelocity, AngularVelocity, "Angular velocity of the object",
      GetAngularVelocity, SetAngularVelocity);

    SF_PRIVATE_PROPERTY_CUSTOM(Physics::RBDynamicState, dynamicState, DynamicState,
      "Body state", GetDynamicState, SetDynamicState);

    SF_PRIVATE_PROPERTY_READ_ONLY(Real, invMass, InverseMass, "Inverse Mass of the object");
  };
}