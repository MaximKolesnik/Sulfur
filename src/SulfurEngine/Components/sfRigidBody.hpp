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

  private:
    SF_PRIVATE_PROPERTY(Vector3, velocity, Velocity, "Velocity of the object");
    SF_PRIVATE_PROPERTY(Vector3, angularVelocity, AngularVelocity, "Angular velocity of the object");
    SF_PRIVATE_PROPERTY(Physics::RBDynamicState, dynamicState, DynamicState, "Body state");

    SF_PRIVATE_PROPERTY(Real, invMass, InverseMass, "Inverse Mass of the object");
  };
}