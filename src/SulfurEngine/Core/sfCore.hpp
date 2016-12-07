/******************************************************************************/
/*!
\par     Sulfur
\file    sfCore.cpp
\author
\par     9/15/2016
\date

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once
#include "Types/sfSingleton.hpp"
#include "Modules/Window/sfWindow.hpp"

namespace Sulfur
{
  class Core
  {
    SF_SINGLETON(Core)

  public:
    void StartUp(HWND windowHandle = nullptr);
    void GameLoop(void);
    void Frame(void);
    void ShutDown(void);

    void OnWindowClose();
    
    Window* GetWindow() const { return m_window; }

  private:
    bool m_running;
    Window *m_window;
  };

}
