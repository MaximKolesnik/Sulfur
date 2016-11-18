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
    SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
    SF_PRIVATE_PROPERTY(Vector3, offset, Offset, "Collider offset from the center");
    SF_PRIVATE_PROPERTY(Real, radius, Radius, "Radius of the collider");
  };
}