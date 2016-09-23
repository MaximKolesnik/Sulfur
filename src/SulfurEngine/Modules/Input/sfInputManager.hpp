/******************************************************************************/
/*!
\par     Sulfur
\file    sfInputManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Input manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "Modules/Window/sfWindow.hpp"

namespace Sulfur
{
	
  class InputManager
  {
    SF_SINGLETON(InputManager)

  private:
    struct InputState
    {
      BYTE Keys[256];
      bool MouseButtons[3];
      POINT MousePosition;
    };

  public:
    void Init(Window *window);
    void Update();

    bool IsKeyDown(BYTE key);
    bool WasKeyPressed(BYTE key);
    bool WasKeyReleased(BYTE key);

    bool IsMouseButtonDown(UINT32 button);
    bool WasMouseButtonPressed(UINT32 button);
    bool WasMouseButtonReleased(UINT32 button);

  private:
    void FillState();

  private:
    Window *m_window;
    InputState m_previousState;
    InputState m_state;

  };
  
}