/******************************************************************************/
/*!
\par     Sulfur
\file    sfScript.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    11/1/2016

\brief   Base script

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfScriptBinding.hpp"
#include "Types/sfObject.hpp"

namespace Sulfur
{
	
class Script
{
  SF_SCRIPT_BASE_CLASS

public:
  Script();

  void Update();

  void RegisterOnInitialize(PyObject *callable);
  void RegisterOnUpdate(PyObject *callable);
  void RegisterEvent(const char *name, PyObject *callable);

public:
  Object *m_owner;

private:
  bool m_initialized;
  PyObject *m_initializeHandler;
  PyObject *m_updateHandler;
  std::unordered_map<std::string, PyObject*> m_eventHandlers;
  
};
  
}