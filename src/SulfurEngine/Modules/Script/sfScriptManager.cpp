/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptManager.cpp
\author  Dylan Norris
\par     DP email: Dylan Norris
\date    10/14/2016

\brief   Python script manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScriptManager.hpp"
#include "sfScriptBinding.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfScriptComponent.hpp"

namespace Sulfur
{

void RegisterScript(PyObject *scriptClass)
{
  if (scriptClass->ob_type != &PyType_Type)
  {
    SF_WARNING("Not a valid script class");
    return;
  }

  PyTypeObject *typeObject = (PyTypeObject*)scriptClass;
  typeObject->tp_members;

  ComponentFactory::Instance()->RegisterScriptType(typeObject->tp_name);
  ScriptComponent *script = static_cast<ScriptComponent*>(ComponentFactory::Instance()->CreateComponent(typeObject->tp_name));
  script->m_name = typeObject->tp_name;
  script->SetPythonType(typeObject);

  ScriptManager::Instance()->RegisterScript(script);
}

SF_SCRIPT_MODULE(sulfur)
  SF_SCRIPT_FUNCTION(RegisterScript)
SF_SCRIPT_MODULE_END()

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{

}

#pragma optimize("", off)
void ScriptManager::Init()
{
  Py_Initialize();

  m_fileWatcher.AddDirectoryToWatch("./Resources");
  m_fileWatcher.RegisterCallbackFileAction(this, &ScriptManager::OnFileAction);

  // Hack to get Script to link
  Script dummyScript;
}
#pragma optimize("", on)

void ScriptManager::Free()
{
  Py_Finalize();
}

void ScriptManager::Update()
{

}

void ScriptManager::OnFileAction(const FileWatcher::ActionInfo& action)
{
  if (action.m_action == FileWatcher::FileAdded || action.m_action == FileWatcher::FileModified)
  {
    RunFile("./Resources/" + action.m_path + "/" + action.m_fileName);
  }
}

void ScriptManager::RegisterScript(ScriptComponent *script)
{
  auto it = m_scripts.find(script->m_name);
  if (it != m_scripts.end())
    ComponentFactory::Instance()->DeleteComponent(it->second);
  else
    m_scriptNames.insert(script->m_name);

  m_scripts[script->m_name] = script;
}

ScriptComponent* ScriptManager::CreateScript(const std::string& name)
{
  auto it = m_scripts.find(name);
  SF_CRITICAL_ERR_EXP(
    it != m_scripts.end(),
    "Script not registered"
    );

  return it->second->Clone();
}

const std::set<std::string>& ScriptManager::GetScriptNames() const
{
  return m_scriptNames;
}

ScriptManager::ModuleTypes& ScriptManager::GetModuleTypes(const std::string& module)
{
  static std::unordered_map<std::string, ModuleTypes> types;

  auto it = types.find(module);
  if (it == types.end())
  {
    types[module] = ModuleTypes();
    it = types.find(module);
  }

  return it->second;
}

ScriptManager::ModuleIntConsts& ScriptManager::GetModuleIntConsts(const std::string& module)
{
  static std::unordered_map<std::string, ModuleIntConsts> consts;

  auto it = consts.find(module);
  if (it == consts.end())
  {
    consts[module] = ModuleIntConsts();
    it = consts.find(module);
  }

  return it->second;
}

ScriptManager::ModuleStringConsts& ScriptManager::GetModuleStringConsts(const std::string& module)
{
  static std::unordered_map<std::string, ModuleStringConsts> consts;

  auto it = consts.find(module);
  if (it == consts.end())
  {
    consts[module] = ModuleStringConsts();
    it = consts.find(module);
  }

  return it->second;
}

void ScriptManager::AddModuleType(const std::string& module, const std::string& name, PyObject *type)
{
  GetModuleTypes(module)[name] = type;
}

void ScriptManager::AddModuleIntConst(const std::string& module, const std::string& name, long value)
{
  GetModuleIntConsts(module)[name] = value;
}

void ScriptManager::AddModuleStringConst(const std::string& module, const std::string& name, const std::string& value)
{
  GetModuleStringConsts(module)[name] = value;
}

void ScriptManager::RunFile(const std::string& path)
{
  FILE *file = fopen(path.c_str(), "rt");
  PyRun_SimpleFile(file, path.c_str());
  fclose(file);
}

}
