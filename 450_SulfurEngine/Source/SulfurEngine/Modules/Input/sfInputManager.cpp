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
  m_previousState = m_state;
  FillState();
}

bool InputManager::IsKeyDown(BYTE key) const
{
  return KEY_DOWN(m_state.Keys[key]);
}


bool InputManager::WasKeyPressed(BYTE key) const
{
  return KEY_DOWN(m_state.Keys[key]) && !KEY_DOWN(m_previousState.Keys[key]);
}

bool InputManager::WasKeyReleased(BYTE key) const
{
  return !KEY_DOWN(m_state.Keys[key]) && KEY_DOWN(m_previousState.Keys[key]);
}

bool InputManager::IsMouseButtonDown(UINT32 button) const
{
  return m_state.MouseButtons[button];
}

bool InputManager::WasMouseButtonPressed(UINT32 button) const
{
  return m_state.MouseButtons[button] && !m_previousState.MouseButtons[button];
}

bool InputManager::WasMouseButtonReleased(UINT32 button) const
{
  return !m_state.MouseButtons[button] && m_previousState.MouseButtons[button];
}

INT32 InputManager::MousePositionX() const
{
  return m_state.MousePosition.x;
}

INT32 InputManager::MousePositionY() const
{
  return m_state.MousePosition.y;
}

INT32 InputManager::MouseDeltaX() const
{
  return m_previousState.MousePosition.x - m_state.MousePosition.x;
}

INT32 InputManager::MouseDeltaY() const
{
  return m_previousState.MousePosition.y - m_state.MousePosition.y;
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
