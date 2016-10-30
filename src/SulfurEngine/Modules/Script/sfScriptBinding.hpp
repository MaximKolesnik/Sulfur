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

namespace Sulfur
{

// Conversions from c types to python objects
template <typename Type>
PyObject *TypeToPyObject(const Type& value);
PyObject *TypeToPyObject();

// Conversions from python objects to c types
template <typename Type>
Type PyObjectToType(PyObject *obj);

// Binding functions, converts python objects to c types for arguments
template <typename ReturnType, ReturnType(*func)()>
PyObject* Bind(PyObject *self, PyObject *args)
{
  return TypeToPyObject((*func)());
}

template <typename ReturnType, typename Arg1, ReturnType(*func)(Arg1)>
PyObject* Bind(PyObject *self, PyObject *args)
{
  return TypeToPyObject((*func)(PyObjectToType<Arg1>(PyTuple_GetItem(args, 0))));
}

template <typename ClassType, typename ReturnType, ReturnType(ClassType::*func)()>
PyObject* BindMember(ClassType *self, PyObject *args)
{
  return TypeToPyObject(((*self).*func)());
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

template <typename C, typename R, typename...Args>
struct FunctionBinder<R(C::*)(Args...)>
{
  template <R(C::*func)(Args...)>
  static PyObject* Bind(C *self, PyObject *args)
  {
    return Sulfur::BindMember<C, R, Args..., func>(self, args);
  }
};
  
}