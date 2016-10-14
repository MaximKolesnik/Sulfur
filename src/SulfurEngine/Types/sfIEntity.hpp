#pragma once

#include <string>

#include "sfTypes.hpp"
#include "Reflection/sfReflection.hpp"
//#include "Events\sfEvents.hpp"

namespace Sulfur
{

  class ComponentFactory;
  class ObjectFactory;
  class Object;
  class DebugDraw;

  SF_REFLECTED_CLASS(IEntity)

  public:
    IEntity(void) : m_hndl(SF_INV_HANDLE), m_owner(SF_INV_HANDLE) {}
    virtual ~IEntity(void) {};

    virtual void Initialize(void) = 0;
    virtual IEntity* Clone(void) const = 0;
    virtual void Update(void) = 0;
    virtual void DrawDebug(DebugDraw *draw) const {}

  protected:
    friend class ComponentFactory;
    friend class ObjectFactory;
    friend class Object;

    SF_PUBLIC_PROPERTY(std::string, name, Name, "Name");
    SF_PROTECTED_PROPERTY(HNDL, hndl, Hndl, "Handle");
    SF_PROTECTED_PROPERTY(HNDL, owner, Owner, "Owner");

    //SF_COMPONENT_EVENTS
  };
}
