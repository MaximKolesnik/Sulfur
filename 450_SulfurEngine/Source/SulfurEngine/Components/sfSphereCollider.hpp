/******************************************************************************/
/*!
\par     Sulfur
\file    sfSphereCollider.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfIEntity.hpp"
#include "Math\sfVector3.hpp"
#include "Reflection\sfReflection.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(SphereCollider, IEntity)
public:
  SphereCollider(void);
  virtual ~SphereCollider(void);

  virtual void Initialize(void) override;
  virtual SphereCollider* Clone(void) const override;
  virtual void Update(void) override;
  virtual void DrawDebug(DebugDraw *draw) const override;

private:
  //SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
  SF_PRIVATE_PROPERTY(Vector3, offset, Offset, "Collider offset from the center");
  SF_PRIVATE_PROPERTY(Real, radius, Radius, 
    "Scale of the collider based on max value from scale of the transform");
};
}