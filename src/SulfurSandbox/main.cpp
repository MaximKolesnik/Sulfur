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
    Sulfur::Physics::PhysicsWorld::Instance()->DebugDrawAABB();
  }
  sulfurEngine->ShutDown();

  return 0;
}