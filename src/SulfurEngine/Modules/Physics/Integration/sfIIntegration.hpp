/******************************************************************************/
/*!
\par     Sulfur
\file    sfIIntegration.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

namespace Sulfur
{
  namespace Physics
  {
    struct RigidBodyData;

    class IIntegration
    {
    public:
      virtual void Integrate(RigidBodyData *bData) = 0;

    private:
    };
  }
}