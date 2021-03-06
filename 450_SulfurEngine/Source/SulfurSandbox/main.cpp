/******************************************************************************/
/*!
\par     Sulfur
\file    main.cpp
\author  Maxim Kolesnik
\par     12/1/2016
\date    12/1/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfSulfurPCH.hpp"
#include "Core/sfCore.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "sfTestLevel.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  Sulfur::Core *sulfurEngine = Sulfur::Core::Instance();

  sulfurEngine->StartUp();
  SetupLevel();
  
  while (true)
  {
    sulfurEngine->Frame();
    Sulfur::Physics::PhysicsWorld::Instance()->DebugDrawColliders();
    //Sulfur::Physics::PhysicsWorld::Instance()->DebugDrawAABB();
  }
  sulfurEngine->ShutDown();

  return 0;
}