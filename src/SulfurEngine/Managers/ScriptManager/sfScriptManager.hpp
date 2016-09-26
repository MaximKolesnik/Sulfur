/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptManager.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/22/2016

\brief   Script manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>

#include "Types\sfSingleton.hpp"
#include "DataStructures\sfSlotMap.hpp"

namespace Sulfur
{
  class IEntity;
  class Compiler;
  class FileWatcher;

  class ScriptManager
  {
    PRIVATE_CTOR_DTOR(ScriptManager);
    FORBID_COPY(ScriptManager);
    SINGLETON_INSTANCE(ScriptManager);

    void Initialize(void);
    void RegisterScript(IEntity *scriptInstance);
    void Update(void);

  public:
  private:
    struct ScriptData
    {
      ISlotMap *m_scriptSlotMap;

    };

    void _CompileAllScripts(void) const;
    int _CompileScript(const std::string &scriptFileName) const;
    std::string _GetDllName(const std::string &file) const;

    void _SetEnvVariables(void) const;

    std::unordered_map<std::string, ScriptData*> m_scriptMap;

    Compiler *m_compiler;
    FileWatcher *m_fileWatcher;
  };

#define SF_SCRIPT(ScriptName)                               \
class ScriptName : public IEntity                           \
{                                                           \
  public:                                                   \
  ScriptName(void) : IEntity()                              \
  {                                                         \
    m_name = #ScriptName;                                   \
  }                                                         \
                                                            \
  virtual ~ScriptName(void);                                \
  virtual void Initialize(void) override final;             \
  virtual IEntity* Clone(void) const override final         \
  {                                                         \
    IEntity *clone = new ScriptName();                      \
    clone->m_name = m_name;                                 \
    return clone;                                           \
  }                                                         \
  virtual void Update(void) overide final;                  \

#define SF_END_SCRIPT(ScriptName) };

#define SF_INIT_SCRIPT(ScriptName)                          \
BOOL APIENTRY DllMain( HMODULE hModule,                     \
  DWORD  ul_reason_for_call,                                \
    LPVOID lpReserved)                                      \
  {                                                         \
static IEntity *instanceForCloning = new ScriptName();      \
    switch (ul_reason_for_call)                             \
    {                                                       \
    case DLL_PROCESS_ATTACH:                                \
      Sulfur::ScriptManager::Instance()->RegisterScript(instanceForCloning); \
      break;                                                \
    case DLL_THREAD_ATTACH:                                 \
      break;                                                \
    case DLL_THREAD_DETACH:                                 \
      break;                                                \
    case DLL_PROCESS_DETACH:                                \
      delete instanceForCloning;                            \
      break;                                                \
    default:                                                \
      break;                                                \
    }                                                       \
    return TRUE;                                            \
  }
}
