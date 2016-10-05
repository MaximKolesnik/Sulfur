#include "sfScriptManager.hpp"
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  SF_DEFINE_TASK(UpdateScripts)
  {
    const ScriptManager::ScriptMap& map = g_SystemTable->ScriptManager->GetScriptMap();

    for (auto scriptIt : map)
    {
      std::string scriptName = scriptIt.second->m_scriptName;
      ComponentFactory::ComponentData data = g_SystemTable->CompFactory->GetComponentData(scriptName);

      for (auto it : data)
      {
        try
        {
          it->Update();
        }
        catch (const std::exception &e)
        {
          std::cout << scriptName << ": " << e.what() << std::endl;
        }
      }
    }
  }
  SF_END_DEFINE_TASK(UpdateScripts)
}