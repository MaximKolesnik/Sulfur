/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptManager.hpp
\author  Dylan Norris
\par     DP email: Dylan Norris
\date    10/14/2016

\brief   Python script manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "FileWatcher/sfFileWatcher.hpp"

namespace Sulfur
{
	
  class ScriptComponent;

  class ScriptManager
  {
    SF_SINGLETON(ScriptManager)

  public:
    typedef std::unordered_map<std::string, PyObject*> ModuleTypes;
    typedef std::unordered_map<std::string, long> ModuleIntConsts;
    typedef std::unordered_map<std::string, std::string> ModuleStringConsts;

  public:
    void Init();
    void Free();

    void Update();

    void OnFileAction(const FileWatcher::ActionInfo& action);

    void RegisterScript(ScriptComponent *script);
    ScriptComponent* CreateScript(const std::string& name);
    const std::set<std::string>& GetScriptNames() const;

    static ModuleTypes& GetModuleTypes(const std::string& module);
    static ModuleIntConsts& GetModuleIntConsts(const std::string& module);
    static ModuleStringConsts& GetModuleStringConsts(const std::string& module);

    static void AddModuleType(const std::string& module, const std::string& name, PyObject *type);
    static void AddModuleIntConst(const std::string& module, const std::string& name, long value);
    static void AddModuleStringConst(const std::string& module, const std::string& name, const std::string& value);

  private:
    void RunFile(const std::string& path);

  private:
    std::set<std::string> m_scriptNames;
    std::unordered_map<std::string, ScriptComponent*> m_scripts;
    FileWatcher m_fileWatcher;

  };
  
}

