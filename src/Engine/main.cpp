#include <iostream>

#include "Core/sfCore.hpp"
#include "Managers\TaskManager\sfTaskManager.hpp"

#pragma optimize("", off)

void testTask2(int counter)
{
  counter = 50;
  std::cout << "Inside job:" << counter << std::endl;
}

int main(int argc, char** argv)
{
  Sulfur::TaskManager *man = Sulfur::TaskManager::Instance();
  int counter = 100;
  man->EnqueueSubTask(testTask2, counter);

  man->RunTasks();

  std::cout << "Outside job:" << counter << std::endl;

  system("pause");
}
#pragma optimize( "", on ) 