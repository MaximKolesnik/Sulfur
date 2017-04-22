/******************************************************************************/
/*!
\par     Sulfur
\file    sfIConstraintSolver.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Modules\Physics\Collision\sfContact.hpp"
#include <unordered_map>

namespace Sulfur
{
  namespace Physics
  {
    /*class IConstraintSolver
    {
    public:
      void AddContact(Contact *contact);

      virtual void PreStep(void) const = 0;
      virtual void Iterate(void) const = 0;

    protected:
      std::unordered_map<ContactKey, Contact> m_contacts;

    private:
      void _UpdateEntry(Contact &contact);
    };*/
  }
}