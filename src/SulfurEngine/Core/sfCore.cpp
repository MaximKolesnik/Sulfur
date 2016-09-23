#include "sfCore.hpp"
#include "Components/sfCamera.hpp"

// Modules
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Logger/sfLogger.hpp"

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
  m_running = true;

  TaskManager* tm = TaskManager::Instance();
  tm->AddNode("UpdateTransforms");
  tm->SetStartingTask("UpdateTransforms");
  tm->CompleteGraph();
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

  ObjectFactory::Instance()->EndFrameCleanUp();
}

void Core::ShutDown(void)
{
}

void Core::OnWindowClose()
{
  m_running = false;
}

}
