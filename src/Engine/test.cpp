#include "test.hpp"
#include <iostream>
#include "Managers\TaskManager\sfTaskManager.hpp"

namespace Sulfur
{
  SF_DEFINE_SUBTASK(SubSubTask, int i)
  {
    std::cout << "SubSubTask " << i << std::endl;
  }SF_END_DEFINE_SUBTASK(SubSubTask);

  SF_DEFINE_SUBTASK(SubTask1, int i)
  {
    SF_ENQUEUE_SUBTASK(SubSubTask, 1);
    SF_WAIT_FOR_SUBTASKS();

    std::cout << "SubTask " << i << std::endl;
  } SF_END_DEFINE_SUBTASK(SubTask1);

  SF_DEFINE_TASK(testTask)
  {
    SF_ENQUEUE_SUBTASK(SubTask1, 1);
    SF_ENQUEUE_SUBTASK(SubTask1, 2);
    SF_ENQUEUE_SUBTASK(SubTask1, 3);
    SF_ENQUEUE_SUBTASK(SubTask1, 4);
    SF_ENQUEUE_SUBTASK(SubTask1, 5);
    SF_WAIT_FOR_SUBTASKS();

    std::cout << "testTask is done" << std::endl;
  } SF_END_DEFINE_TASK(testTask);
}