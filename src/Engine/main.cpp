#include <iostream>

#include "Core/sfCore.hpp"
#include "Managers\TaskManager\sfTaskManager.hpp"

#include "test.hpp"

#pragma optimize("", off)

int main(int argc, char** argv)
{
  Sulfur::TaskManager *man = Sulfur::TaskManager::Instance();
  
  man->AddIndependentNode("testTask");
  man->CompleteGraph();

  man->RunTasks();

  system("pause");
}
#pragma optimize( "", on ) 