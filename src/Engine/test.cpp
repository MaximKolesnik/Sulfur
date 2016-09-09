#include "test.hpp"
#include <iostream>
#include "Managers\TaskManager\sfTaskManager.hpp"

namespace Sulfur
{
  SF_DEFINE_TASK(testTask)
  {
    std::cout << "testTask started" << std::endl;

    std::cout << "testTask is done" << std::endl;
  } SF_END_DEFINE_TASK(testTask);

  SF_DEFINE_TASK(testTask2)
  {
    std::cout << "testTask2 started" << std::endl;

    std::cout << "testTask2 is done" << std::endl;
  } SF_END_DEFINE_TASK(testTask2);

  SF_DEFINE_TASK(testTask3)
  {
    std::cout << "testTask3 started" << std::endl;

    std::cout << "testTask3 is done" << std::endl;
  } SF_END_DEFINE_TASK(testTask3);

  SF_DEFINE_TASK(testTask4)
  {
    std::cout << "testTask4 started" << std::endl;

    std::cout << "testTask4 is done" << std::endl;
  } SF_END_DEFINE_TASK(testTask4);
}