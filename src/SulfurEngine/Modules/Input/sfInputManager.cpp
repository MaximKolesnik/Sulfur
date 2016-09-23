/******************************************************************************/
/*!
\par     Sulfur
\file    sfInputManager.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Input manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfInputManager.hpp"

#define KEY_DOWN(val) ((val & 0x80) != 0)
namespace Sulfur
{

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Init(Window *window)
{
  m_window = window;

  FillState();
  m_previousState = m_state;
}

void InputManager::Update()
{
  if (m_window->IsFocused())
  {
    m_previousState = m_state;
    FillState();
  }
}

bool InputManager::IsKeyDown(BYTE key)
{
  return KEY_DOWN(m_state.Keys[key]);
}


bool InputManager::WasKeyPressed(BYTE key)
{
  return KEY_DOWN(m_state.Keys[key]) && !KEY_DOWN(m_previousState.Keys[key]);
}

bool InputManager::WasKeyReleased(BYTE key)
{
  return !KEY_DOWN(m_state.Keys[key]) && KEY_DOWN(m_previousState.Keys[key]);
}

bool InputManager::IsMouseButtonDown(UINT32 button)
{
  return m_state.MouseButtons[button];
}

bool InputManager::WasMouseButtonPressed(UINT32 button)
{
  return m_state.MouseButtons[button] && !m_previousState.MouseButtons[button];
}

bool InputManager::WasMouseButtonReleased(UINT32 button)
{
  return !m_state.MouseButtons[button] && m_previousState.MouseButtons[button];
}

void InputManager::FillState()
{
  GetKeyboardState(m_state.Keys);
  m_state.MouseButtons[0] = IsKeyDown(VK_LBUTTON);
  m_state.MouseButtons[1] = IsKeyDown(VK_MBUTTON);
  m_state.MouseButtons[2] = IsKeyDown(VK_RBUTTON);

  GetCursorPos(&m_state.MousePosition);
  ScreenToClient(m_window->GetHandle(), &m_state.MousePosition);
}

}
