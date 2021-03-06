/******************************************************************************/
/*!
\par     Sulfur
\file    sfWindowManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Window manager

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "sfWindow.hpp"

namespace Sulfur
{
	
  class WindowManager
  {
    SF_SINGLETON(WindowManager);
  public:
    Window* AddWindow(HWND handle);
    Window* NewWindow(const WindowDescription& description);

    void Update();

  private:
    std::vector<Window *> m_windows;
  };
  
}