/******************************************************************************/
/*!
\par     Sulfur
\file    sfWindow.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Window instance

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfWindowDescription.hpp"

namespace Sulfur
{
	
// TODO: Add messaging callbacks
  class Window
  {

  public:
    Window();
    Window(HWND handle);
    Window(const WindowDescription& description);
    ~Window();

    void Init(HWND handle);
    void Init(const WindowDescription& description);
    void Free();

    HWND GetHandle() const;

    bool IsClosed() const;

  private:
    HWND CreateWnd(const WindowDescription& description);

    LRESULT MessageHandler(UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    bool m_closed;
    HWND m_handle;

  };
  
}