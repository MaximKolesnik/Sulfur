/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptManager.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/22/2016

\brief   Script manager
         When new script is added, it is going to be compiled as soon
         as both cpp and hpp are located
         Script is recompiled when either hpp or cpp is modified
         When script is modified recompilation is attempted. If there are any
         errors, last working dll version will be used.
         If hpp or cpp is removed, script is removed completely
         Renaming will only change internal data

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>

#include "Types\sfSingleton.hpp"
#include "DataStructures\sfSlotMap.hpp"
#include "FileWatcher\sfFileWatcher.hpp"

namespace Sulfur
{
  class IEntity;
  class Compiler;

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
    friend void FileWatcherCallback(const FileWatcher::ActionInfo &);

    struct ScriptData;
    typedef std::unordered_map<std::string, ScriptData*> ScriptMap;

    struct ScriptData
    {
      ScriptData() : m_header(""), m_cpp(""), m_libHandle(NULL),
        m_compiled(false) {};

      std::string m_header;
      std::string m_cpp;
      std::string m_dllName;
      std::string m_relativePath; //relative to ScriptSourceDir
      
      HMODULE m_libHandle;

      bool m_compiled;
    };

    std::string _GetDllName(const std::string &file) const;
    void _HandleFileAddedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileModifiedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRemovedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRenamedOldAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRenamedNewAction(const FileWatcher::ActionInfo &actionInfo);

    bool _IsHeader(const std::string &fileName) const;
    std::string _RemoveExtension(const std::string &fileName) const;

    std::vector<std::string> _GatherHeadersPathes(void) const;
    std::vector<std::string> _GatherCppsPathes(void) const;

    ScriptMap m_scriptMap;

    Compiler *m_compiler;
    FileWatcher *m_fileWatcher;
  };

#define SF_SCRIPT(ScriptName)                               \
class ScriptName : public IEntity     \
{                                                           \
  public:                                                   \
  ScriptName(void) : IEntity()                              \
  {                                                         \
    m_name = #ScriptName;                                   \
  }                                                         \
                                                            \
  virtual ~ScriptName(void);                                \
  _declspec(dllexport) virtual void Initialize(void) override final;             \
  _declspec(dllexport) virtual IEntity* Clone(void) const override final         \
  {                                                         \
    IEntity *clone = new ScriptName();                      \
    clone->m_name = m_name;                                 \
    return clone;                                           \
  }                                                         \
  _declspec(dllexport) virtual void Update(void) override final;                  \

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
