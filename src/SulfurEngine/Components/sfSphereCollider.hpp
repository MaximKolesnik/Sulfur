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

  const Vector3& GetOffset(void) const;
  const Real& GetRadius(void) const;

  void SetOffset(const Vector3 &offset);
  void SetRadius(const Real &rad);

private:
  //SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
  SF_PRIVATE_PROPERTY_CUSTOM(Vector3, offset, Offset, "Collider offset from the center",
    GetOffset, SetOffset);
  SF_PRIVATE_PROPERTY_CUSTOM(Real, radius, Radius, 
    "Scale of the collider based on max value from scale of the transform",
    GetRadius, SetRadius);
};
}