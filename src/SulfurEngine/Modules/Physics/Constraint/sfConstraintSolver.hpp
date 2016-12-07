#pragma once

#include "Modules\Physics\Collision\sfIContactGenerator.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class ConstraintSolver
    {
    public:
      void Solve(Contacts &contacts) const;
    private:
      void _PrepareContacts(Contacts &contacts) const;
      void _ApplyImpulses(Contacts &contacts) const;
      void _PostChange(Contacts &contact) const;
    };
  }
}