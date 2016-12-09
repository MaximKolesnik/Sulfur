/******************************************************************************/
/*!
\par     Sulfur
\file    sfConstraintSolver.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

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