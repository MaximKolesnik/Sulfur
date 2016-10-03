#pragma once

#include "Types\sfIEntity.hpp"
#include "SystemTable\sfSystemTable.hpp"
#include "Managers\ScriptManager\sfScriptRegistry.hpp"

namespace Sulfur
{
  class IScript : public IEntity
  {
  public:
    IScript(void) : IEntity()
    {

    }

    virtual ~IScript(void) {};

    virtual void Initialize(void) override = 0;
    virtual IScript* Clone(void) const override = 0;
    virtual void Update(void) override = 0;
  protected:
    static SystemTable* Engine;
  };
}