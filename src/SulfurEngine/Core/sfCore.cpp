#include "sfCore.hpp"
#include "Components/sfCamera.hpp"

// Modules
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
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
  SystemTable* const g_SystemTable = new SystemTable;

  Core::Core()
  {
  }

  Core::~Core()
  {
  }

  void Core::StartUp(HWND windowHandle)
  {
    _InitiazlieSystemTable();

    // Start engine in existing window
    if (windowHandle != nullptr)
      m_window = WindowManager::Instance()->AddWindow(windowHandle);
    
    // Create a new window for the engine
    else
    {
      WindowDescription description;
      description.Width = 1280;
      description.Height = 720;
      description.Title = "Sulfur Engine";

      m_window = WindowManager::Instance()->NewWindow(description);
      m_window->RegisterCallbackOnClose(this, &Core::OnWindowClose);
    }

    InputManager::Instance()->Init(m_window);
    GraphicsManager::Instance()->Init(*m_window);
    ScriptManager::Instance()->Initialize();
    m_running = true;

    TaskManager* tm = TaskManager::Instance();
    tm->AddNode("UpdateTransforms");
    tm->AddNode("UpdateScripts");
    tm->SetStartingTask("UpdateScripts");
    tm->SetDependency("UpdateTransforms", "UpdateScripts");
    tm->CompleteGraph();

    MeshRenderer *meshRenderer = ComponentFactory::Instance()->CreateComponent<MeshRenderer>();
    meshRenderer->SetMesh(ResourceManager<Mesh>::Instance()->LoadResource("Models/cube.fbx"));

    Object *testObj = ObjectFactory::Instance()->CreateObject();
    testObj->AttachComponent(meshRenderer);
    testObj->AttachComponent(ComponentFactory::Instance()->CreateComponent("TestScript"));
    SceneManager::Instance()->GetScene().AddObject(testObj->GetHndl());
  }

  void Core::GameLoop(void)
  {

    while (m_running)
      Frame();
  }

  void Core::Frame(void)
  {
    WindowManager::Instance()->Update();
    InputManager::Instance()->Update();
    TaskManager::Instance()->RunTasks();
    GraphicsManager::Instance()->Update();

    if (InputManager::Instance()->WasKeyPressed('A'))
      std::cout << "A pressed" << std::endl;

      ScriptManager::Instance()->Update();
    ObjectFactory::Instance()->EndFrameCleanUp();
  }

  void Core::ShutDown(void)
  {
  }

  void Core::OnWindowClose()
  {
    m_running = false;
  }

  void Core::_InitiazlieSystemTable(void) const
  {
    const_cast<ComponentFactory*>(g_SystemTable->CompFactory) = ComponentFactory::Instance();
    const_cast<ObjectFactory*>(g_SystemTable->ObjFactory) = ObjectFactory::Instance();
  }

}
