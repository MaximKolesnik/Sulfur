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

    const Vector3& GetOffset(void) const;
    const Real& GetRadius(void) const;
    const Real& GetCenterLineLength(void) const;

    void SetOffset(const Vector3 &offset);
    void SetRadius(const Real &rad);
    void SetCenterLineLength(const Real &length);
  private:
    //SF_PRIVATE_PROPERTY(bool, isGhost, IsGhost, "Ghost collider");
    SF_PRIVATE_PROPERTY_CUSTOM(Vector3, offset, Offset, "Collider offset from the center",
      GetOffset, SetOffset);
    SF_PRIVATE_PROPERTY_CUSTOM(Real, radius, Radius, "Radius of the collider",
      GetRadius, SetRadius);
    SF_PRIVATE_PROPERTY_CUSTOM(Real, centerLineLength, CenterLineLength, "Length of the center line",
      GetCenterLineLength, SetCenterLineLength);
  };
}