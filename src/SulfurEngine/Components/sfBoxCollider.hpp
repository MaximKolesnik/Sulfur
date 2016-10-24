#pragma once

#include "Types\sfIEntity.hpp"
#include "Math\sfVector3.hpp"
#include "Reflection\sfReflection.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(BoxCollider, IEntity)
  public:
    BoxCollider(void);
    virtual ~BoxCollider(void);

    virtual void Initialize(void) override;
    virtual BoxCollider* Clone(void) const override;
    virtual void Update(void) override;
    virtual void DrawDebug(DebugDraw *draw) const override;

  private:
    SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
    SF_PRIVATE_PROPERTY(Vector3, offset, Offset, "Collider offset from the center");
    SF_PRIVATE_PROPERTY(Vector3, scale, Scale, "Scale of the collider");
  };
}