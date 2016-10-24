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