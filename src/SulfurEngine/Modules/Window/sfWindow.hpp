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
#include "Utils/sfClassUtils.hpp"

namespace Sulfur
{
	
  class Window
  {

  public:
    Window();
    Window(HWND handle);
    Window(const WindowDescription& description);
    ~Window();

    void Init(HWND handle);
    void Init(const WindowDescription& description);
    void Update();
    void Free();

    void OnSize(UINT32 width, UINT32 height);

    bool IsFocused() const;

    HWND GetHandle() const;

  private:
    HWND CreateWnd(const WindowDescription& description);

    LRESULT MessageHandler(UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  private:
    HWND m_handle;
    UINT32 m_size[2];

    // Callback functions
    SF_CALLBACK(OnSize, UINT32, UINT32)
    SF_CALLBACK(OnClose)

  };
  
}