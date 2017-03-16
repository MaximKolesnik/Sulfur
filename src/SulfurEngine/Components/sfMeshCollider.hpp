/******************************************************************************/
/*!
\par     Sulfur
\file    sfMeshCollider.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    1/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Types\sfIEntity.hpp"
#include "Reflection\sfReflection.hpp"
#include "Math\sfVector3.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(MeshCollider, IEntity)
  public:
    MeshCollider(void);
    virtual ~MeshCollider(void);

    virtual void Initialize(void) override final;
    virtual MeshCollider* Clone(void) const override final;
    virtual void Update(void) override;
    virtual void DrawDebug(DebugDraw *draw) const override;

    void SetColliderMesh(const std::string &meshName);

  private:
    SF_RESOURCE(Mesh, mesh, Mesh, "Mesh");
    SF_PRIVATE_PROPERTY(Vector3, offset, Offset, "Collider offset from the center");
    SF_PRIVATE_PROPERTY(Vector3, scale, Scale, "Collider scale");
  };
}