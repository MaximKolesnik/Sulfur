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