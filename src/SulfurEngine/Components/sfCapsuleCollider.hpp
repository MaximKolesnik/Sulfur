/******************************************************************************/
/*!
\par     Sulfur
\file    sfCapsuleCollider.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/15/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfIEntity.hpp"
#include "Math\sfVector3.hpp"
#include "Reflection\sfReflection.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(CapsuleCollider, IEntity)
  public:
    CapsuleCollider(void);
    virtual ~CapsuleCollider(void);

    virtual void Initialize(void) override final;
    virtual CapsuleCollider* Clone(void) const override final;
    virtual void Update(void) override final;
    virtual void DrawDebug(DebugDraw *draw) const override final;

  private:
    //SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
    SF_PRIVATE_PROPERTY(Vector3, offset, Offset, "Collider offset from the center");
    SF_PRIVATE_PROPERTY(Real, radius, Radius, "Radius of the collider");
    SF_PRIVATE_PROPERTY(Real, centerLineLength, CenterLineLength, "Length of the center line");
  };
}