#include "sfCore.hpp"
#include "Components/sfTransform.hpp"

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

  Transform transform;
  transform.SetPropertyValue("Name", std::string("TransformName"));

  transform.SetTranslation(Vector3(100.0f, 0.0f, 0.0f));
  transform.SetRotationEulerXZY(0.0f, 0.0f, 0.0f);
  transform.SetScale(Vector3(1.0f, 1.0f, 1.0f));

  std::ofstream file("test.bin", std::ios_base::binary);
  Serialization::Serialize(file, transform);
  file.close();

  Transform transform2;
  std::ifstream infile("test.bin", std::ios_base::binary);
  Serialization::Deserialize(infile, transform2);

  ReflectionBase::PropertyList properties;
  Transform::Properties(properties);

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
