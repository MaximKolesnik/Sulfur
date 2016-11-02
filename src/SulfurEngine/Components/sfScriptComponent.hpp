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
#pragma once
#include "Types/sfIEntity.hpp"
#include "Managers/TaskManager/sfTaskRegistry.hpp"
#include "Modules/Script/sfScript.hpp"

namespace Sulfur
{

SF_DECLARE_TASK(UpdateScripts)

SF_REFLECTED_CLASS_DERIVED(ScriptComponent, IEntity)

public:
  ScriptComponent();
  ~ScriptComponent();

  void SetPythonType(PyTypeObject *pythonType, bool createInstance = false);

  virtual void Initialize(void) override final;
  virtual ScriptComponent* Clone(void) const override final;
  virtual void Update(void) override;

private:
  bool m_initialized;
  PyTypeObject *m_pythonType;
  Script *m_instance;

};
  
}