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

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <unordered_map>

#include "Types\sfSingleton.hpp"
#include "DataStructures\sfSlotMap.hpp"
#include "FileWatcher\sfFileWatcher.hpp"
#include "Types\sfIScript.hpp"
#include "SystemTable\sfSystemTable.hpp"
#include "Managers/TaskManager/sfTaskRegistry.hpp"

namespace Sulfur
{
  SF_DECLARE_TASK(UpdateScripts)

  class IEntity;
  class Compiler;

  typedef ISlotMap* (*RegFunc)(void *);
  typedef char* (*RegName)(void);

  class ScriptManager
  {
    SF_PRIVATE_CTOR_DTOR(ScriptManager);
    SF_FORBID_COPY(ScriptManager);
    SF_SINGLETON_INSTANCE(ScriptManager);

    void Initialize(void);
    void RegisterScript(IEntity *scriptInstance);
    void Update(void);

  public:
    struct ScriptData
    {
      ScriptData() : m_header(""), m_cpp(""), m_relativePath(""), m_libHandle(NULL),
        m_compiled(false) {};

      std::string m_header;
      std::string m_cpp;
      std::string m_dllName;
      std::string m_relativePath; //relative to ScriptSourceDir
      std::string m_scriptName;

      HMODULE m_libHandle;

      bool m_compiled;
    };

    typedef std::unordered_map<std::string, ScriptData*> ScriptMap;

    const ScriptMap& GetScriptMap(void) { return m_scriptMap; }

  private:
    friend void FileWatcherCallback(const FileWatcher::ActionInfo &);
    friend class ComponentFactory;

    std::string _GetDllName(const std::string &file) const;
    void _HandleFileAddedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileModifiedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRemovedAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRenamedOldAction(const FileWatcher::ActionInfo &actionInfo);
    void _HandleFileRenamedNewAction(const FileWatcher::ActionInfo &actionInfo);

    bool _IsHeader(const std::string &fileName) const;
    std::string _RemoveExtension(const std::string &fileName) const;
    void _DeleteFile(const std::string &file) const;
    void _RenameFiles(const std::string &dllName,
      const std::string &newName) const;

    void _InitializeScriptData(void);
    void _LocateScripts(LPCTSTR folder);

    std::vector<std::string> _GatherHeadersPathes(void) const;
    std::vector<std::string> _GatherCppsPathes(void) const;

    ScriptMap m_scriptMap;

    Compiler *m_compiler;
    FileWatcher *m_fileWatcher;

    std::string m_scriptFolder;
  };

#define SF_SCRIPT(ScriptName)                               \
namespace __RegHelpers {extern "C" {__declspec(dllexport) Sulfur::ISlotMap* __SFRegisterS(void *);}}  \
class ScriptName : public Sulfur::IScript                   \
{                                                           \
  friend Sulfur::ISlotMap* __RegHelpers::__SFRegisterS(void *);      \
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

#define SF_END_SCRIPT(ScriptName) }; \
namespace __RegHelpers               \
  {\
    extern "C" \
    {__declspec(dllexport) char* __SFScriptName(void); }\
  }
  

#define SF_INIT_SCRIPT(ScriptName)                          \
Sulfur::SystemTable * Sulfur::IScript::Engine = nullptr;  \
namespace __RegHelpers {Sulfur::ISlotMap* __SFRegisterS(void *st)\
{                                                           \
  Sulfur::ISlotMap* sm = new Sulfur::SlotMap<ScriptName>(); \
  ScriptName::Engine = reinterpret_cast<Sulfur::SystemTable*>(st);\
  return sm;                                                \
}                                                           \
char* __SFScriptName(void)                                  \
{                                                           \
  return #ScriptName;                                       \
}}

}
