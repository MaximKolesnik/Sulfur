#include "sfCore.hpp"
#include "Components/sfCamera.hpp"

// Modules
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Logger/sfLogger.hpp"

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
  TaskManager::Instance()->RunTasks();
  GraphicsManager::Instance()->Update();
}

void Core::ShutDown(void)
{
}

void Core::OnWindowClose()
{
  m_running = false;
}

}
