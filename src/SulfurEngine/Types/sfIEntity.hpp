#pragma once

#include <string>
#include "sfTypes.hpp"
#include "Reflection/sfReflection.hpp"

namespace Sulfur
{

  class ComponentFactory;
  class ObjectFactory;
  class Object;

  SF_REFLECTED_CLASS(IEntity)

  public:
    IEntity(void) : m_hndl(SF_INV_HANDLE), m_owner(SF_INV_HANDLE) {}
    virtual ~IEntity(void) {};

    virtual void Initialize(void) = 0;
    virtual IEntity* Clone(void) const = 0;

    HNDL GetHndl(void) const
    {
      return m_hndl;
    }

    HNDL GetOwner(void) const
    {
      return m_owner;
    }

    SF_PUBLIC_PROPERTY(std::string, name, Name, "Name");

  protected:
    friend class ComponentFactory;
    friend class ObjectFactory;
    friend class Object;

    HNDL m_owner;

    HNDL m_hndl;
  };
}
