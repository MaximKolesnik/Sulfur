/******************************************************************************/
/*!
\par     Sulfur
\file    sfScript.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/1/2016

\brief   Base script

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScript.hpp"
#include "sfScriptBinding.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfScriptComponent.hpp"

namespace Sulfur
{

SF_SCRIPT_TYPE(sulfur, Script)
  SF_SCRIPT_MEMBER_FUNCTION(RegisterOnInitialize)
  SF_SCRIPT_MEMBER_FUNCTION(RegisterOnUpdate)
  SF_SCRIPT_MEMBER_FUNCTION(RegisterEvent)
  SF_SCRIPT_MEMBER(m_owner, owner)
SF_SCRIPT_TYPE_END()

Script::Script()
  : m_owner(nullptr), m_initialized(false), m_initializeHandler(nullptr), m_updateHandler(nullptr)
{
}

void Script::Update()
{
  if (!m_initialized)
  {
    if (m_initializeHandler) PyObject_CallObject(m_initializeHandler, PyTuple_Pack(1, &ob_base));
    m_initialized = true;
  }
  else
  {
    if (m_updateHandler) PyObject_CallObject(m_updateHandler, PyTuple_Pack(1, &ob_base));
  }
}

void Script::RegisterOnInitialize(PyObject *callable)
{
  m_initializeHandler = callable;
}

void Script::RegisterOnUpdate(PyObject *callable)
{
  m_updateHandler = callable;
}

void Script::RegisterEvent(const char *name, PyObject *callable)
{
  std::cout << name << std::endl;
  m_eventHandlers[name] = callable;
}

}
