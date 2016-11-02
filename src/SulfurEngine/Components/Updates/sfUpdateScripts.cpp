/******************************************************************************/
/*!
\par     Sulfur
\file    sfUpdateScripts.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/31/2016

\brief   Script update tasks

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Components/sfScriptComponent.hpp"
#include "Modules/Script/sfScriptManager.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

  SF_DEFINE_TASK(UpdateScripts)
  {
    auto& scriptTypes = ScriptManager::Instance()->GetScriptNames();
    for (const std::string& scriptType : scriptTypes)
    {
      ComponentFactory::ComponentData data = ComponentFactory::Instance()->GetComponentData(scriptType);
      for (auto it = data.begin(); it != data.end(); ++it)
      {
        it->Update();
      }
    }
  }
  SF_END_DEFINE_TASK(UpdateScripts)

}
