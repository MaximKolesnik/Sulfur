#pragma once

namespace Sulfur
{
  class ObjectFactory;
  class ComponentFactory;
  class Logger;
  class ScriptManager;
  class TaskManager;
  class WindowManager;
  class InputManager;
  class SceneManager;
  class GraphicsManager;
  class Time;

  struct SystemTable
  {
    SystemTable() : Log(nullptr), ObjFactory(nullptr), CompFactory(nullptr),
      InputManager(nullptr), SceneManager(nullptr), ScriptManager(nullptr), 
      TaskManager(nullptr), WindowManager(nullptr), GraphicsManager(nullptr),
      Time(nullptr) {};

    Logger* Log;
    ObjectFactory* ObjFactory;
    ComponentFactory* CompFactory;
    InputManager* InputManager;
    SceneManager* SceneManager;
    Time* Time;

#ifdef SF_BUILD_SCRIPT //Systems below should not be accessible in scripts
  private:
#endif
    ScriptManager* ScriptManager;
    TaskManager* TaskManager;
    WindowManager* WindowManager;
    GraphicsManager* GraphicsManager;
  };
  
  extern SystemTable* g_SystemTable;
}