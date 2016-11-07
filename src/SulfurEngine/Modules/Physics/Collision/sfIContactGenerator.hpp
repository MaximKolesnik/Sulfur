#pragma once

#include "Math\sfVector3.hpp"

namespace Sulfur
{
  class QueryResults;

  namespace Physics
  {
    struct ColliderData;

    struct Contact
    {
      ColliderData *m_colliderA;
      ColliderData *m_colliderB;

      Vector3 m_contactNormal;
      Vector3 m_contactPoint;
      Vector3 m_tangent;

      Real m_penetration;

      Real m_massNormal;
      Real m_massTangent1;
      Real m_massTangent2;

      Real m_bias;
    };

    typedef std::vector<Contact> Contacts;

    class IContactGenerator
    {
    public:
      IContactGenerator(void) {}
      virtual ~IContactGenerator(void) {}

      virtual void BoxToBox(Contacts &contacts,
        ColliderData *colliderA, ColliderData *colliderB) const = 0;
    private:
    };
  }
}