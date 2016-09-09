#include <iostream>

#include "Core/sfCore.hpp"
#include "Managers\TaskManager\sfTaskManager.hpp"

#include "test.hpp"

#pragma optimize("", off)

using namespace Sulfur;


int main(int argc, char** argv)
{
  Sulfur::TaskManager *man = Sulfur::TaskManager::Instance();
  
  man->AddIndependentNode("testTask");
  man->AddDependentNode("testTask2", "testTask");
  man->AddDependentNode("testTask3", "testTask");
  man->AddDependentNode("testTask4", "testTask3");
  man->CompleteGraph();

  int i = 0;
  while (i < 2)
  { 
    man->RunTasks();
  }

  system("pause");
}
#pragma optimize( "", on ) 