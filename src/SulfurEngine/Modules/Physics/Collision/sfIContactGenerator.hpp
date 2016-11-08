#pragma once

#include "Math\sfVector3.hpp"

namespace Sulfur
{
  class QueryResults;

  namespace Physics
  {
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