/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptBinding.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/19/2016

\brief   Script binding helpers

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfScriptManager.hpp"

namespace Sulfur
{

// Conversions from c types to python objects
template <typename Type>
PyObject *TypeToPyObject(const Type& value);
PyObject *TypeToPyObject();

// Conversions from python objects to c types
template <typename Type>
Type PyObjectToType(PyObject *obj);

template <typename Type>
Type* OffsetPtr(Type *ptr)
{
  return reinterpret_cast<Type*>(reinterpret_cast<char*>(ptr) - offsetof(Type, ob_base));
}

template <typename Type>
Type* OffsetPtrRev(Type *ptr)
{
  return reinterpret_cast<Type*>(reinterpret_cast<char*>(ptr) + offsetof(Type, ob_base));
}

// Binding functions, converts python objects to c types for arguments
template <typename ReturnType, ReturnType(*func)()>
PyObject* Bind(PyObject *self, PyObject *args)
{
  return TypeToPyObject((*func)());
}

template <typename ReturnType, typename Arg1, ReturnType(*func)(Arg1)>
PyObject* Bind(PyObject *self, PyObject *args)
{
  return TypeToPyObject<ReturnType>((*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0))));
}

template <typename Arg1, void(*func)(Arg1)>
PyObject* BindNoReturn(PyObject *self, PyObject *args)
{
  (*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0)));
  Py_RETURN_NONE;
}

template <typename ClassType, typename ReturnType, ReturnType(ClassType::*func)()>
PyObject* BindMember(ClassType *self, PyObject *args)
{
  return TypeToPyObject(((*OffsetPtr(self)).*func)());
}

template <typename ClassType, typename ReturnType, ReturnType(ClassType::*func)() const>
PyObject* BindMember(ClassType *self, PyObject *args)
{
  return TypeToPyObject(((*OffsetPtr(self)).*func)());
}

template <typename ClassType, void(ClassType::*func)()>
PyObject* BindMemberNoReturn(ClassType *self, PyObject *args)
{
  ((*OffsetPtr(self)).*func)();
  Py_RETURN_NONE;
}

template <typename ClassType, typename Arg1, void(ClassType::*func)(Arg1)>
PyObject* BindMemberNoReturn(ClassType *self, PyObject *args)
{
  ((*OffsetPtr(self)).*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0)));
  Py_RETURN_NONE;
}

template <typename ClassType, typename Arg1, void(ClassType::*func)(Arg1) const>
PyObject* BindMemberNoReturn(ClassType *self, PyObject *args)
{
  ((*OffsetPtr(self)).*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0)));
  Py_RETURN_NONE;
}

template <typename ClassType, typename Arg1, typename Arg2, void(ClassType::*func)(Arg1, Arg2)>
PyObject* BindMemberNoReturn(ClassType *self, PyObject *args)
{
  ((*OffsetPtr(self)).*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0)), PyObjectToType<Arg2>(PyTuple_GetItem(args, 1)));
  Py_RETURN_NONE;
}

template <typename ClassType, typename Arg1, typename Arg2, void(ClassType::*func)(Arg1, Arg2) const>
PyObject* BindMemberNoReturn(ClassType *self, PyObject *args)
{
  ((*OffsetPtr(self)).*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0)), PyObjectToType<Arg2>(PyTuple_GetItem(args, 1)));
  Py_RETURN_NONE;
}

// Templated binder helpers
template <typename T>
struct FunctionBinder;

template <typename R, typename...Args>
struct FunctionBinder<R(*)(Args...)>
{
  template <R(*func)(Args...)>
  static PyObject* Bind(PyObject *self, PyObject *args)
  {
    return Sulfur::Bind<R, Args..., func>(self, args);
  }
};

template <typename...Args>
struct FunctionBinder<void(*)(Args...)>
{
  template <void(*func)(Args...)>
  static PyObject* Bind(PyObject *self, PyObject *args)
  {
    return Sulfur::BindNoReturn<Args..., func>(self, args);
  }
};

template <typename C, typename R, typename...Args>
struct FunctionBinder<R(C::*)(Args...)>
{
  template <R(C::*func)(Args...)>
  static PyObject* Bind(C *self, PyObject *args)
  {
    return Sulfur::BindMember<C, R, Args..., func>(self, args);
  }
};

template <typename C, typename R, typename...Args>
struct FunctionBinder<R(C::*)(Args...) const>
{
  template <R(C::*func)(Args...) const>
  static PyObject* Bind(C *self, PyObject *args)
  {
    return Sulfur::BindMember<C, R, Args..., func>(self, args);
  }
};

template <typename C, typename...Args>
struct FunctionBinder<void(C::*)(Args...)>
{
  template <void(C::*func)(Args...)>
  static PyObject* Bind(C *self, PyObject *args)
  {
    return Sulfur::BindMemberNoReturn<C, Args..., func>(self, args);
  }
};

template <typename C, typename...Args>
struct FunctionBinder<void(C::*)(Args...) const>
{
  template <void(C::*func)(Args...) const>
  static PyObject* Bind(C *self, PyObject *args)
  {
    return Sulfur::BindMemberNoReturn<C, Args..., func>(self, args);
  }
};

template <typename T>
struct PythonMemberType
{
  static const int Type;
};

template <typename T>
const int PythonMemberType<T>::Type = T_OBJECT;

template <typename Type>
struct PythonTypeData
{
  static PyTypeObject TypeObject;
};

#define SF_PYTHON_MEMBER_TYPE(ctype, ptype) template <> const int PythonMemberType<ctype>::Type = ptype;

// Module and type macros

// Must be the first line of the class declaration
#define SF_SCRIPT_BASE_CLASS public: PyObject_HEAD private:

#define SF_SCRIPT_TYPE_EX(module, type, flags, baseType) \
int Init##type(type *self, PyObject *args, PyObject *kwds) { return 0; } \
PyObject* New##type(PyTypeObject *to, PyObject *args, PyObject *kwds) { type* obj = (type*)to->tp_alloc(to, 0); new (obj) type(); return (PyObject*)OffsetPtrRev(obj); } \
template <> PyTypeObject Sulfur::PythonTypeData<type>::TypeObject = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    #type, \
    sizeof(type), \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    flags, \
    "", \
    0, 0, 0, 0, 0, 0, 0, 0, 0, \
    baseType, \
    0, 0, 0, 0, \
    (initproc)Init##type, \
    0, \
    New##type, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; \
namespace type##Binding { \
  typedef type ScriptType; \
  const std::string TypeName = #type; \
  const std::string ModuleName = #module; \
  std::vector<PyMethodDef> Methods; \
  std::vector<PyMemberDef> Members;
  
#define SF_SCRIPT_TYPE_ABSTRACT_EX(module, type, flags, baseType) \
template <> PyTypeObject Sulfur::PythonTypeData<type>::TypeObject = { \
    PyVarObject_HEAD_INIT(NULL, 0) \
    #type, \
    sizeof(type), \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    flags, \
    "", \
    0, 0, 0, 0, 0, 0, 0, 0, 0, \
    baseType, \
    0, 0, 0, 0, \
    0, \
    0, \
    0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; \
namespace type##Binding { \
  typedef type ScriptType; \
  const std::string TypeName = #type; \
  const std::string ModuleName = #module; \
  std::vector<PyMethodDef> Methods; \
  std::vector<PyMemberDef> Members;

#define SF_SCRIPT_TYPE(module, type) SF_SCRIPT_TYPE_EX(module, type, Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, 0)
#define SF_SCRIPT_TYPE_DERIVED(module, type, baseType) SF_SCRIPT_TYPE_EX(module, type, Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, &Sulfur::PythonTypeData<baseType>::TypeObject)
#define SF_SCRIPT_TYPE_ABSTRACT(module, type) SF_SCRIPT_TYPE_ABSTRACT_EX(module, type, Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, 0)
#define SF_SCRIPT_TYPE_ABSTRACT_DERIVED(module, type, baseType) SF_SCRIPT_TYPE_ABSTRACT_EX(module, type, Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, &Sulfur::PythonTypeData<baseType>::TypeObject)

#define SF_SCRIPT_MEMBER_FUNCTION(func) \
  static PyMethodDef Register##func() { Methods.push_back(PyMethodDef{ #func, (PyCFunction)FunctionBinder<decltype(&ScriptType::func)>::Bind<&ScriptType::func>, METH_VARARGS }); return Methods.back(); } \
  static PyMethodDef func##Def = Register##func();

#define SF_SCRIPT_MEMBER(member, pythonName) \
  static PyMemberDef Register##member() { Members.push_back(PyMemberDef{ #pythonName, PythonMemberType<decltype(ScriptType::##member)>::Type, offsetof(ScriptType, member) - offsetof(ScriptType, ob_base), 0, #pythonName }); return Members.back(); } \
  static PyMemberDef member##Def = Register##member();

#define SF_SCRIPT_TYPE_END() \
  static int Register() { \
    Methods.push_back(PyMethodDef{ nullptr }); \
    Members.push_back(PyMemberDef{ nullptr }); \
    Sulfur::PythonTypeData<ScriptType>::TypeObject.tp_methods = Methods.data(); \
    Sulfur::PythonTypeData<ScriptType>::TypeObject.tp_members = Members.data(); \
    ScriptManager::AddModuleType(ModuleName.c_str(), TypeName, (PyObject*)&Sulfur::PythonTypeData<ScriptType>::TypeObject); \
    return 0; \
  } \
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
    Methods.push_back(PyMethodDef{ nullptr }); \
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
  
}