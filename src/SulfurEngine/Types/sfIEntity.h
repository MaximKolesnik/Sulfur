#pragma once

#include <string>
#include "sfTypes.hpp"
#include "Reflection/sfReflection.hpp"

namespace Sulfur
{

  SF_REFLECTED_CLASS(IEntity)

  public:
    IEntity(void) {}
    virtual ~IEntity(void) {};

    virtual void Initialize(void) = 0;
    virtual void Update(Real dt) = 0;
    virtual IEntity* Clone(void) const = 0;

    HNDL GetOwner(void) const
    {
      return m_hndl;
    }

    SF_PUBLIC_PROPERTY(std::string, name, Name, "Name");

  //protected:
    HNDL m_hndl;
  };
}
