#include <iostream>

#include "Core/sfCore.hpp"
#include "Managers\TaskManager\sfTaskManager.hpp"

#include "test.hpp"

#pragma optimize("", off)

using namespace Sulfur;


int main(int argc, char** argv)
{
  Sulfur::TaskManager *man = Sulfur::TaskManager::Instance();
  
  man->AddNode("testTask");
  man->SetStartingTask("testTask");
  man->CompleteGraph();

  int i = 0;
  while (i < 2)
  { 
    man->RunTasks();
    std::cout << "-----------------------" << std::endl;
    ++i;
  }

  system("pause");
}
#pragma optimize( "", on ) 