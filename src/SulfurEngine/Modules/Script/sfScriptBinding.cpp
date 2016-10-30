/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptBinding.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/19/2016

\brief   Script binding helpers

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScriptBinding.hpp"

namespace Sulfur
{

PyObject* TypeToPyObject()
{
  Py_RETURN_NONE;
}

template <>
PyObject *TypeToPyObject<bool>(const bool& value)
{
  return PyLong_FromLong((long)value);
}

template <>
PyObject *TypeToPyObject<INT32>(const INT32& value)
{
  return PyLong_FromLong((long)value);
}

template <>
PyObject *TypeToPyObject<PyObject*>(PyObject* const & value)
{
  return value;
}

template <>
void PyObjectToType<void>(PyObject *obj)
{
}

template <>
bool PyObjectToType<bool>(PyObject *obj)
{
  return PyLong_AsLong(obj) != 0;
}

template <>
INT32 PyObjectToType<INT32>(PyObject *obj)
{
  return (INT32)PyLong_AsLong(obj);
}

template <>
PyObject* PyObjectToType<PyObject*>(PyObject *obj)
{
  return obj;
}

}
