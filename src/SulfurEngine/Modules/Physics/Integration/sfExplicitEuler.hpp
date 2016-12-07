/******************************************************************************/
/*!
\par     Sulfur
\file    sfExplicitEuler.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "sfIIntegration.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class ExplicitEuler : public IIntegration
    {
    public:
      virtual void Integrate(RigidBodyData *bData) override;

    private:
    };
  }
}