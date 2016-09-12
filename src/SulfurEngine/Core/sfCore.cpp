#include "sfCore.hpp"

// Modules
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
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
  }

  GraphicsManager::Instance()->Init(*m_window);
}

void Core::GameLoop(void)
{
  m_running = true;
  while (m_running)
  {
    WindowManager::Instance()->Update();
    GraphicsManager::Instance()->Update();

    if (m_window->IsClosed())
      m_running = false;
  }
}

void Core::ShutDown(void)
{
}

}
