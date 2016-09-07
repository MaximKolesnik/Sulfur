/******************************************************************************/
/*!
\par     Sulfur
\file    sfWindowManager.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Window manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfWindowManager.hpp"

namespace Sulfur
{

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
  for (Window *window : m_windows)
    delete window;
}

Window* WindowManager::AddWindow(HWND handle)
{
  Window *window = new Window(handle);
  m_windows.push_back(window);

  return window;
}

Window* WindowManager::NewWindow(const WindowDescription& description)
{
  Window *window = new Window(description);
  m_windows.push_back(window);

  return window;
}

void WindowManager::Update()
{
  MSG msg = { 0 };
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

}
