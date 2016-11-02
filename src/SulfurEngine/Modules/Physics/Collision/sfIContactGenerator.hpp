#pragma once

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