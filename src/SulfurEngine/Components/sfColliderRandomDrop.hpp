/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderRandomDrop.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    12/8/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfIEntity.hpp"
#include "Reflection\sfReflection.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(ColliderRandomDrop, IEntity)
  public:
    ColliderRandomDrop(void);
    virtual ~ColliderRandomDrop(void);

    virtual void Initialize(void) override final;
    virtual ColliderRandomDrop* Clone(void) const override final;
    virtual void Update(void) override final;
    virtual void DrawDebug(DebugDraw *draw) const override final;

  private:
    SF_PRIVATE_PROPERTY_RANGE(Real, delta, Delta, "Drop timer", Real(1.0), Real(10.0));
    SF_PRIVATE_PROPERTY_RANGE(Real, dropHeight, DropHeight, "Drop Height", Real(1.0), Real(20.0));

    Real m_elapsed;
  };
}