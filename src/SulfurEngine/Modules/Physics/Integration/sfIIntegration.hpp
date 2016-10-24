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