#pragma once

#include "sfIContactGenerator.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class SAT : public IContactGenerator
    {
    public:
      SAT(void) : IContactGenerator() {}
      virtual ~SAT(void) {}

      virtual void GenerateContacts(Contacts &contacts, 
                                    const QueryResults &possiblePairs) override;
    private:
    };
  }
}