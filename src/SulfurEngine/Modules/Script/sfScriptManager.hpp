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

#define SF_SCRIPT_TYPE(module, type) \
namespace type##Binding { \
  typedef type ScriptType; \
  const std::string TypeName = #type; \
  const std::string ModuleName = #module; \
  std::vector<PyMethodDef> Methods; \
  int Init(type *self, PyObject *args, PyObject *kwds) { new (self) type(); return 0; } \
  PyTypeObject TypeObject = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    #type, \
    sizeof(type), \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, \
    "", \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    (initproc)Init, \
    0, \
    PyType_GenericNew, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define SF_SCRIPT_MEMBER_FUNCTION(func) \
  static PyMethodDef Register##func() { Methods.push_back(PyMethodDef{ #func, (PyCFunction)FunctionBinder<decltype(&ScriptType::func)>::Bind<&ScriptType::func>, METH_VARARGS }); return Methods.back(); } \
  static PyMethodDef func##Def = Register##func();

#define SF_SCRIPT_TYPE_END() \
  static int Register() { Methods.push_back(PyMethodDef{nullptr}); TypeObject.tp_methods = Methods.data(); ScriptManager::AddModuleType(ModuleName.c_str(), TypeName, (PyObject*)&TypeObject); return 0; } \
  static int Registered = Register(); \
}

#define SF_SCRIPT_MODULE(module) \
namespace module##ModuleBinding { \
  const std::string ModuleName = #module; \
  std::vector<PyMethodDef> Methods; \
  PyModuleDef Module = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    #module, \
    0, \
    -1, \
    0, 0, 0, 0, 0 \
  }; \
  PyMODINIT_FUNC Init() { \
    Methods.push_back(PyMethodDef{nullptr}); \
    Module.m_methods = Methods.data(); \
    PyObject *module = PyModule_Create(&Module); \
    ScriptManager::ModuleTypes& types = ScriptManager::GetModuleTypes(ModuleName); \
    for (auto& type : types) { PyType_Ready((PyTypeObject*)type.second); Py_INCREF(type.second); PyModule_AddObject(module, type.first.c_str(), type.second); } \
    ScriptManager::ModuleIntConsts& intConsts = ScriptManager::GetModuleIntConsts(ModuleName); \
    for (auto& c : intConsts) { PyModule_AddIntConstant(module, c.first.c_str(), c.second); } \
    ScriptManager::ModuleStringConsts& stringConsts = ScriptManager::GetModuleStringConsts(ModuleName); \
    for (auto& c : stringConsts) { PyModule_AddStringConstant(module, c.first.c_str(), c.second.c_str()); } \
    return module; \
  }
  
#define SF_SCRIPT_FUNCTION(func) \
  static PyMethodDef Register##func() { Methods.push_back(PyMethodDef{ #func, (PyCFunction)FunctionBinder<decltype(&func)>::Bind<&func>, METH_VARARGS }); return Methods.back(); } \
  static PyMethodDef func##Def = Register##func();

#define SF_SCRIPT_INT_CONST(name, value) \
  static long Register##name() { ScriptManager::AddModuleIntConst(ModuleName, #name, (long)value); return (long)value; } \
  static long name##Registered = Register##name();

#define SF_SCRIPT_STRING_CONST(name, value) \
  static std::string Register##name() { ScriptManager::AddModuleStringConst(ModuleName, #name, value); return value; } \
  static std::string name##Registered = Register##name();

#define SF_SCRIPT_MODULE_END() \
  static int Register() { PyImport_AppendInittab(ModuleName.c_str(), &Init); return 0; } \
  static int Registered = Register(); \
}

namespace Sulfur
{
	
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

    static ModuleTypes& GetModuleTypes(const std::string& module);
    static ModuleIntConsts& GetModuleIntConsts(const std::string& module);
    static ModuleStringConsts& GetModuleStringConsts(const std::string& module);

    static void AddModuleType(const std::string& module, const std::string& name, PyObject *type);
    static void AddModuleIntConst(const std::string& module, const std::string& name, long value);
    static void AddModuleStringConst(const std::string& module, const std::string& name, const std::string& value);

  };

  class TestModule
  {
  public:
    TestModule() : i(15) {}
    int PrintNum() { std::cout << i << std::endl; return i; }

  private:
    int i;
  };
  
}

