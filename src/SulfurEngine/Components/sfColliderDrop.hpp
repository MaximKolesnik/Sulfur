/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderDrop.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    3/11/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "Types\sfIEntity.hpp"
#include "Reflection\sfReflection.hpp"
#include "Math\sfVector3.hpp"

namespace Sulfur
{
  SF_REFLECTED_CLASS_DERIVED(ColliderDrop, IEntity)
  public:
    ColliderDrop(void);
    virtual ~ColliderDrop(void);

    virtual void Initialize(void) override final;
    virtual ColliderDrop* Clone(void) const override final;
    virtual void Update(void) override final;
    virtual void DrawDebug(DebugDraw *draw) const override final;

  private:
    SF_PRIVATE_PROPERTY(Vector3, dropOffset, DropOffset, "Drop Offset");

    static const int c_numInRow = 2;
    HNDL m_createdObjects[c_numInRow][c_numInRow][c_numInRow];

    bool m_created = false;

    void _CreateSpheres(void);
    void _CreateBoxes(void);
    void _DestroyObjects(void);
  };
}