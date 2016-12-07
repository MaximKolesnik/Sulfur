/******************************************************************************/
/*!
\par     Sulfur
\file    main.cpp
\author  Maxim Kolesnik
\par     12/1/2016
\date    DP email: maxim.kolesnik@digipen.edu

\brief   

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSulfurPCH.hpp"
#include "Core/sfCore.hpp"
#include "Factories\sfComponentFactory.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  Sulfur::Core *sulfurEngine = Sulfur::Core::Instance();

  sulfurEngine->StartUp();
  sulfurEngine->GameLoop();
  sulfurEngine->ShutDown();

  return 0;
}