#include "sfCore.hpp"
#include "Components/sfCamera.hpp"

// Modules
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Time/sfTime.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Logger/sfLogger.hpp"
#include "Managers\ScriptManager\sfScriptManager.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules\Scene\sfSceneManager.hpp"

// Factories
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

#include "SystemTable\sfSystemTable.hpp"
#include "Components\sfMeshRenderer.hpp"

namespace Sulfur
{
  Core::Core()
  {
  }

  Core::~Core()
  {
  }

  void Core::StartUp(HWND windowHandle)
  {
    //Init system table
    g_SystemTable = new SystemTable();
    g_SystemTable->Log = new Logger();

    g_SystemTable->CompFactory = new ComponentFactory();
    g_SystemTable->CompFactory->Initialize();

    g_SystemTable->ObjFactory = new ObjectFactory();
    g_SystemTable->ScriptManager = new ScriptManager();
    g_SystemTable->TaskManager = new TaskManager();
    g_SystemTable->WindowManager = new WindowManager();
    g_SystemTable->InputManager = new InputManager();
    g_SystemTable->SceneManager = new SceneManager();
    g_SystemTable->GraphicsManager = new GraphicsManager();
    g_SystemTable->Time = new Time();

    // Start engine in existing window
    if (windowHandle != nullptr)
      m_window = g_SystemTable->WindowManager->AddWindow(windowHandle);
    
    // Create a new window for the engine
    else
    {
      WindowDescription description;
      description.Width = 1280;
      description.Height = 720;
      description.Title = "Sulfur Engine";

      m_window = g_SystemTable->WindowManager->NewWindow(description);
      m_window->RegisterCallbackOnClose(this, &Core::OnWindowClose);
    }

    g_SystemTable->InputManager->Init(m_window);
    g_SystemTable->GraphicsManager->Init(*m_window);
    g_SystemTable->ScriptManager->Initialize();
    m_running = true;

    TaskManager* tm = g_SystemTable->TaskManager;
    tm->AddNode("UpdateTransforms");
    tm->AddNode("UpdateScripts");
    tm->SetStartingTask("UpdateScripts");
    tm->SetDependency("UpdateTransforms", "UpdateScripts");
    tm->CompleteGraph();
  }

  void Core::GameLoop(void)
  {

    while (m_running)
      Frame();
  }

  void Core::Frame(void)
  {
    g_SystemTable->WindowManager->Update();
    g_SystemTable->InputManager->Update();
    g_SystemTable->TaskManager->RunTasks();
    g_SystemTable->GraphicsManager->Update();
    g_SystemTable->ScriptManager->Update();
    g_SystemTable->ObjFactory->EndFrameCleanUp();
    g_SystemTable->Time->WaitForFrameRate();
  }

  void Core::ShutDown(void)
  {
  }

  void Core::OnWindowClose()
  {
    m_running = false;
  }

}
