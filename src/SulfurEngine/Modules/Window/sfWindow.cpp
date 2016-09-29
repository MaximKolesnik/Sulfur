/******************************************************************************/
/*!
\par     Sulfur
\file    sfWindow.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Window instance

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfWindow.hpp"

namespace Sulfur
{

Window::Window()
  : m_handle(nullptr)
{
}

Window::Window(HWND handle)
{
  Init(handle);
}

Window::Window(const WindowDescription& description)
{
  Init(description);
}

Window::~Window()
{
  Free();
}

void Window::Init(HWND handle)
{
  m_handle = handle;
}

void Window::Init(const WindowDescription& description)
{
  Init(CreateWnd(description));
  UpdateWindow(m_handle);
  ShowWindow(m_handle, SW_SHOW);
  memset(m_size, 0, sizeof(m_size));
}

void Window::Update()
{
  MSG msg = { 0 };
  while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  RECT currentRect;
  GetClientRect(m_handle, &currentRect);
  UINT32 width = currentRect.right - currentRect.left;
  UINT32 height = currentRect.bottom - currentRect.top;

  if ((width != m_size[0] || height != m_size[1]) && width != 0 && height != 0)
  {
    m_size[0] = width; m_size[1] = height;
    NotifyOnSize(width, height);
  }
}

void Window::Free()
{
  if (m_handle) 
    DestroyWindow(m_handle);
}
 
bool Window::IsFocused() const
{
  return (GetFocus() == m_handle);
}

HWND Window::GetHandle() const
{
  return m_handle;
}

LRESULT Window::MessageHandler(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CREATE:
    break;

  case WM_CLOSE:
    NotifyOnClose();
    DestroyWindow(m_handle);
    m_handle = nullptr;
    break;
  }

  return DefWindowProc(m_handle, msg, wParam, lParam);
}

HWND Window::CreateWnd(const WindowDescription& description)
{
  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = 0;
  wc.lpfnWndProc = &WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = description.Title.c_str();
  wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  SF_CRITICAL_ERR_EXP(
    RegisterClassEx(&wc) != 0,
    "Failed to register window class"
    );

  // Step 2: Creating the Window
  CreateWindowEx(
    WS_EX_CLIENTEDGE,
    description.Title.c_str(),
    description.Title.c_str(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, description.Width, description.Height,
    NULL, NULL, wc.hInstance, this);

  SF_CRITICAL_ERR_EXP(
    m_handle != nullptr,
    "Failed to create new window"
    );

  SF_LOG_MESSAGE("Window successfully created");

  return m_handle;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  Window *window = nullptr;

  if (msg == WM_NCCREATE)
  {
    window = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
    window->m_handle = hwnd;

    SetLastError(0);
    if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window)))
    {
      if (GetLastError() != 0)
        return FALSE;
    }
  }
  else
  {
    window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
  }

  if (window)
    return window->MessageHandler(msg, wParam, lParam);

  return DefWindowProc(hwnd, msg, wParam, lParam);
}

}
