/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptComponent.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/14/2016

\brief   Script component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScriptComponent.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Modules/Script/sfScriptBinding.hpp"

namespace Sulfur
{

ScriptComponent::ScriptComponent()
  : m_pythonType(nullptr), m_initialized(true)
{
}

ScriptComponent::~ScriptComponent()
{
  if (m_pythonType)
    Py_DECREF(m_pythonType);
}

void ScriptComponent::SetPythonType(PyTypeObject *pythonType, bool createInstance)
{
  if (m_pythonType) 
    Py_DECREF(m_pythonType);

  if (m_instance)
  {
    Py_DECREF(m_instance);
    m_instance = nullptr;
  }

  if (pythonType)
  {
    Py_INCREF(pythonType);

    if (createInstance)
      m_instance = (Script*)PyObject_CallObject((PyObject*)pythonType, nullptr);
  }

  m_pythonType = pythonType;
  PyTypeObject *base = (PyTypeObject*)PyTuple_GetItem(m_pythonType->tp_bases, 0);
  m_initialized = false;
}

void ScriptComponent::Initialize(void)
{

}

ScriptComponent* ScriptComponent::Clone(void) const
{
  ScriptComponent *newScript = static_cast<ScriptComponent*>(ComponentFactory::Instance()->CreateComponent(m_name));
  newScript->m_name = m_name;
  newScript->SetPythonType(m_pythonType, true);
  return newScript;
}

void ScriptComponent::Update(void)
{
  if (m_instance)
  {
    m_instance->m_owner = ObjectFactory::Instance()->GetObject(m_owner);
    m_instance->Update();
  }
}

}
