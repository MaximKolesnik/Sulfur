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

namespace Sulfur
{

bool RegisterScript(PyObject *scriptClass)
{
  static std::vector<PyTypeObject*> scripts;
  scripts.push_back((PyTypeObject*)scriptClass);
  PyObject *keys = PyDict_Keys(scripts.back()->tp_dict);
  int keyCount = (int)PyList_Size(keys);

  for (int i = 0; i < keyCount; ++i)
  {
    PyObject *key = PyList_GetItem(keys, i);
    PyObject *value = PyDict_GetItem(scripts.back()->tp_dict, key);
    char *keyChar = PyUnicode_AsUTF8(key);

    if (value->ob_type != &PyFunction_Type && keyChar[0] != '_')
    {
      std::cout << PyUnicode_AsUTF8(key) << std::endl;
      PyDict_SetItem(scripts.back()->tp_dict, key, PyLong_FromLong(10));
    }
  }

  return true;
}

SF_SCRIPT_TYPE(sulfur, TestModule)
  SF_SCRIPT_MEMBER_FUNCTION(PrintNum)
SF_SCRIPT_TYPE_END()

SF_SCRIPT_MODULE(sulfur)
  SF_SCRIPT_INT_CONST(test_constant, 5)
  SF_SCRIPT_STRING_CONST(test_string_constant, "A test string")
  SF_SCRIPT_FUNCTION(RegisterScript)
SF_SCRIPT_MODULE_END()

ScriptManager::ScriptManager()
{

}

ScriptManager::~ScriptManager()
{

}

void ScriptManager::Init()
{
  Py_Initialize();

  FILE *file = fopen("Resources/Scripts/TestScript.py", "rt");
  int result = PyRun_SimpleFile(file, "TestScript.py");
  fclose(file);
}

void ScriptManager::Free()
{
  Py_Finalize();
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

}
