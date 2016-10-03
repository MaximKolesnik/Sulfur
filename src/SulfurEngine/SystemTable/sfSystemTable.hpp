#pragma once

namespace Sulfur
{
  class ObjectFactory;
  class ComponentFactory;

  struct SystemTable
  {
    SystemTable() : ObjFactory(nullptr), CompFactory(nullptr) {};

    ObjectFactory* const ObjFactory;
    ComponentFactory* const CompFactory;
  };
  
  extern SystemTable* const g_SystemTable;
}