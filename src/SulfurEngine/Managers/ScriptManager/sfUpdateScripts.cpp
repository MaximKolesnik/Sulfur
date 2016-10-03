#include "sfScriptManager.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  SF_DEFINE_TASK(UpdateScripts)
  {
    const ScriptManager::ScriptMap& map = ScriptManager::Instance()->GetScriptMap();

    for (auto scriptIt : map)
    {
      std::string scriptName = scriptIt.second->m_scriptName;
      ComponentFactory::ComponentData data = ComponentFactory::Instance()->GetComponentData(scriptName);

      for (auto it : data)
        it->Update();
    }
  }
  SF_END_DEFINE_TASK(UpdateScripts)
}