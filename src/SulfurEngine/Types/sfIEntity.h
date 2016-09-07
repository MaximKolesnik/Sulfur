#pragma once

#include <string>

#include "sfTypes.hpp"

namespace Sulfur
{
  class IEntity
  {
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

    const std::string& GetName(void) const
    {
      return m_name;
    }

  //protected:
    std::string m_name;
    HNDL m_hndl;
  };
}