#include "sfNarrowPhase.hpp"
#include "Modules\Physics\Collision\sfSAT.hpp"

namespace Sulfur
{
  namespace Physics
  {


    NarrowPhase::NarrowPhase(void)
    {
      m_contactGenerator = new SAT;

      m_collisionFunction[CT_BOX][CT_BOX] = &IContactGenerator::BoxToBox;
      m_collisionFunction[CT_BOX][CT_SPHERE] = &IContactGenerator::BoxToSphere;
      m_collisionFunction[CT_BOX][CT_CAPSULE] = &IContactGenerator::BoxToCapsule;
      m_collisionFunction[CT_SPHERE][CT_BOX] = &IContactGenerator::SphereToBox;
      m_collisionFunction[CT_SPHERE][CT_SPHERE] = &IContactGenerator::SphereToSphere;
      m_collisionFunction[CT_SPHERE][CT_CAPSULE] = &IContactGenerator::SphereToCapsule;
      m_collisionFunction[CT_CAPSULE][CT_CAPSULE] = &IContactGenerator::CapsuleToCapsule;
      m_collisionFunction[CT_CAPSULE][CT_BOX] = &IContactGenerator::CapsuleToBox;
      m_collisionFunction[CT_CAPSULE][CT_SPHERE] = &IContactGenerator::CapsuleToSphere;
    }

    NarrowPhase::~NarrowPhase(void)
    {
      delete m_contactGenerator;
    }

    void NarrowPhase::Collide(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB) const
    {
      IContactGenerator::CollisionFunc func = m_collisionFunction[colliderA->m_type][colliderB->m_type];

      ((*m_contactGenerator).*func)(contacts, colliderA, colliderB);
    }
  }
}