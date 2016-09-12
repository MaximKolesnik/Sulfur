#pragma once
#include <iostream>
#include "Types/sfSingleton.h"
#include "Modules/Window/sfWindow.hpp"

namespace Sulfur
{

  class Core
  {
    SINGLETON(Core)

  public:
    void StartUp(HWND windowHandle = nullptr);
    void GameLoop(void);
    void Frame(void);
    void ShutDown(void);

    void OnWindowClose();

  private:
    bool m_running;
    Window *m_window;

  };

}
