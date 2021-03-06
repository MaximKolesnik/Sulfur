/******************************************************************************/
/*!
\par     Sulfur
\file    sfCore.cpp
\author  
\par     9/15/2016
\date    

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfCore.hpp"
#include "Components/sfCamera.hpp"

// Modules
#include "Modules/Vr/sfVrManager.hpp"
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Time/sfTime.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Logger/sfLogger.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "Modules/Physics/sfPhysicsWorld.hpp"
#include "Managers\EventManager\sfEventManager.hpp"

// Factories
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

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
    Logger::Instance()->Initialize();
    ComponentFactory::Instance()->Initialize();
    TaskManager::Instance()->Initialize();

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
    //VrManager::Instance()->Init();
    Physics::PhysicsWorld::Instance()->Initialize();

    m_running = true;

    TaskManager* tm = TaskManager::Instance();
    tm->AddNode("UpdateTransforms");
    tm->AddNode("RenderGraphics");
    tm->AddNode("UpdateGameplay");
    tm->AddNode("TriggerEventsEndFrame");
    tm->AddNode("SyncData");
    tm->AddNode("IntegrateBodies");
    tm->AddNode("BroadPhase");
    tm->AddNode("NarrowPhase");
    tm->AddNode("PostAndCleanup");
    tm->SetStartingTask("UpdateTransforms");
    tm->SetDependency("UpdateGameplay", "UpdateTransforms");
    tm->SetDependency("RenderGraphics", "UpdateGameplay");
    tm->SetDependency("SyncData", "RenderGraphics");
    tm->SetDependency("BroadPhase", "SyncData");
    tm->SetDependency("NarrowPhase", "BroadPhase");
    tm->SetDependency("IntegrateBodies", "NarrowPhase");
    tm->SetDependency("PostAndCleanup", "IntegrateBodies");    
    tm->SetDependency("TriggerEventsEndFrame", "PostAndCleanup");
    tm->CompleteGraph();

    /*Object *testObj = SF_CREATE_OBJECT("testEvent");
    EventManager::Instance()->PushEvent(IEntity::&OnTestEvent, OnTestEventData(testObj->GetHndl(), "Test"));*/
  }

  void Core::GameLoop(void)
  {
    while (m_running)
      Frame();
  }

  void Core::Frame(void)
  {
    WindowManager::Instance()->Update();
    //VrManager::Instance()->Update();
    InputManager::Instance()->Update();
    SceneManager::Instance()->Update();
    TaskManager::Instance()->RunTasks();
    ObjectFactory::Instance()->EndFrameCleanUp();
    Time::Instance()->WaitForFrameRate();
  }

  void Core::ShutDown(void)
  {
    GraphicsManager::Instance()->Free();
  }

  void Core::OnWindowClose()
  {
    m_running = false;
  }

}
