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

template <> PyObject *TypeToPyObject<bool>(const bool& value) { return PyLong_FromLong((long)value); }
template <> PyObject *TypeToPyObject<INT16>(const INT16& value) { return PyLong_FromLong((long)value); }
template <> PyObject *TypeToPyObject<UINT16>(const UINT16& value) { return PyLong_FromUnsignedLong((unsigned long)value); }
template <> PyObject *TypeToPyObject<INT32>(const INT32& value) { return PyLong_FromLong((long)value); }
template <> PyObject *TypeToPyObject<UINT32>(const UINT32& value) { return PyLong_FromUnsignedLong((unsigned long)value); }
template <> PyObject *TypeToPyObject<INT64>(const INT64& value) { return PyLong_FromLongLong((long long)value); }
template <> PyObject *TypeToPyObject<UINT64>(const UINT64& value) { return PyLong_FromUnsignedLongLong((unsigned long long)value); }
template <> PyObject *TypeToPyObject<char *>(char * const & value) { return PyUnicode_FromString(value); }
template <> PyObject *TypeToPyObject<const char *>(const char * const & value) { return PyUnicode_FromString(value); }
template <> PyObject *TypeToPyObject<PyObject*>(PyObject* const & value) { return value; }

template <> void PyObjectToType<void>(PyObject *obj) { }
template <> bool PyObjectToType<bool>(PyObject *obj) { return PyLong_AsLong(obj) != 0; }
template <> INT16 PyObjectToType<INT16>(PyObject *obj) { return (INT16)PyLong_AsLong(obj); }
template <> UINT16 PyObjectToType<UINT16>(PyObject *obj) { return (UINT16)PyLong_AsUnsignedLong(obj); }
template <> INT32 PyObjectToType<INT32>(PyObject *obj) { return (INT32)PyLong_AsLong(obj); }
template <> UINT32 PyObjectToType<UINT32>(PyObject *obj) { return (UINT32)PyLong_AsUnsignedLong(obj); }
template <> INT64 PyObjectToType<INT64>(PyObject *obj) { return (INT64)PyLong_AsLongLong(obj); }
template <> UINT64 PyObjectToType<UINT64>(PyObject *obj) { return (UINT64)PyLong_AsUnsignedLongLong(obj); }
template <> char * PyObjectToType<char *>(PyObject *obj) { return PyUnicode_AsUTF8(obj); }
template <> const char * PyObjectToType<const char *>(PyObject *obj) { return PyUnicode_AsUTF8(obj); }
template <> PyObject* PyObjectToType<PyObject*>(PyObject *obj) { return obj; }

SF_PYTHON_MEMBER_TYPE(bool, T_BOOL)
SF_PYTHON_MEMBER_TYPE(INT8, T_CHAR)
SF_PYTHON_MEMBER_TYPE(UINT8, T_BYTE)
SF_PYTHON_MEMBER_TYPE(INT16, T_SHORT)
SF_PYTHON_MEMBER_TYPE(UINT16, T_USHORT)
SF_PYTHON_MEMBER_TYPE(INT32, T_INT)
SF_PYTHON_MEMBER_TYPE(UINT32, T_UINT)
SF_PYTHON_MEMBER_TYPE(INT64, T_LONGLONG)
SF_PYTHON_MEMBER_TYPE(UINT64, T_ULONGLONG)
SF_PYTHON_MEMBER_TYPE(float, T_FLOAT)
SF_PYTHON_MEMBER_TYPE(double, T_DOUBLE)
SF_PYTHON_MEMBER_TYPE(char*, T_STRING_INPLACE)
SF_PYTHON_MEMBER_TYPE(const char*, T_STRING_INPLACE)

}
