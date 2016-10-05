#pragma once

#include "Types\sfIEntity.hpp"
#include "Managers\ScriptManager\sfScriptRegistry.hpp"
#include "DataStructures\sfSlotMap.hpp"

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
  };

  typedef ISlotMap* (*RegFunc)(void *);
  typedef char* (*RegName)(void);

  namespace FunctionTable
  {
    typedef std::unordered_map<std::string, std::pair<RegFunc, RegName> > FUNCTABLE;

    inline FUNCTABLE& GetTable(void)
    {
      static FUNCTABLE table;
      return table;
    }

    template <class Type>
    class Entry
    {
    public:
      Entry(const std::string &name, RegFunc regFunc, RegName nameFunc)
      {
        FUNCTABLE &table = GetTable();
        table.emplace(name, std::make_pair(regFunc, nameFunc));
      }
    };
  }

  typedef const FunctionTable::FUNCTABLE& (*getTableFunc)();

#ifdef SF_BUILD_SCRIPT
  extern "C"
  {
    __declspec(dllexport) const FunctionTable::FUNCTABLE& SFGetTable()
    {
      return FunctionTable::GetTable();
    }
  }
#endif

#define SF_SCRIPT(ScriptName)                               \
namespace __RegHelpers                                          \
  {                                                             \
     char* __SF##ScriptName##Name(void); \
Sulfur::ISlotMap* __SFReg##ScriptName(void *); \
  }                                                                               \
class ScriptName : public Sulfur::IScript                   \
{                                                           \
  friend Sulfur::ISlotMap* __RegHelpers::__SFReg##ScriptName(void *);      \
  public:                                                   \
  ScriptName(void) : IScript()                              \
  {                                                         \
    m_name = #ScriptName;                                   \
  }                                                         \
                                                            \
  virtual ~ScriptName(void);                                \
  virtual void Initialize(void) override final;             \
  virtual ScriptName* Clone(void) const override final      \
  {                                                         \
    return nullptr;                                         \
  }                                                         \
  virtual void Update(void) override final;                 

#define SF_END_SCRIPT(ScriptName) };                            \
namespace Sulfur {namespace FunctionTable {                                       \
  namespace                                                                       \
  {                                                                               \
  template <class Type>                                                           \
  class FuncRegistration;                                                         \
  template <>                                                                     \
  class FuncRegistration<ScriptName>                                              \
  {                                                                               \
    static const Entry<ScriptName> &entry;                                        \
  };                                                                              \
  const Entry<ScriptName> &FuncRegistration<ScriptName>::entry                    \
  = Entry<ScriptName>(std::string(__FILE__), __RegHelpers::__SFReg##ScriptName, \
  __RegHelpers::__SF##ScriptName##Name); } } }

#define SF_INIT_SCRIPT(ScriptName)                          \
namespace __RegHelpers {Sulfur::ISlotMap* __SFReg##ScriptName(void *st)\
{                                                           \
  Sulfur::ISlotMap* sm = new Sulfur::SlotMap<ScriptName>(); \
  Sulfur::g_SystemTable = reinterpret_cast<Sulfur::SystemTable*>(st);\
  return sm;                                                \
}                                                           \
char* __SF##ScriptName##Name(void)                          \
{                                                           \
  return #ScriptName;                                       \
}}
}