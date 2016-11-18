#pragma once

#include "Modules\Physics\Collision\sfContact.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Physics\Collision\sfIContactGenerator.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class NarrowPhase
    {
    public:
      NarrowPhase(void);
      ~NarrowPhase(void);

      void Collide(Contacts &contacts, ColliderData *colliderA, 
        ColliderData *colliderB) const;

    private:
      IContactGenerator::CollisionFunc 
        m_collisionFunction[CT_NUM_TYPES][CT_NUM_TYPES];

      IContactGenerator *m_contactGenerator;
    };
  }
}