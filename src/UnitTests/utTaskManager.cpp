#include "stdafx.h"
#include "CppUnitTest.h"

#include <stack>
#include <vector>
#include <algorithm>

#include "Types/sfTypes.hpp"
#include "Managers\TaskManager\sfTaskManager.hpp"
#include "TaskManagerTestFuncs.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sulfur;

std::vector<int> taskNumCompletion;
int counter = 0;

std::mutex testMutex;

namespace Sulfur
{
#pragma optimize( "", off )
  SF_DEFINE_TASK(TestTask1)
  {
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(1);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask1);

  SF_DEFINE_TASK(TestTask2)
  {
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(2);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask2);

  SF_DEFINE_TASK(TestTask3)
  {
    Sleep(4000); 
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(3);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask3);

  SF_DEFINE_TASK(TestTask4)
  {
    Sleep(8000);
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(4);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask4);

  SF_DEFINE_TASK(TestTask5)
  {
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(5);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask5);

  SF_DEFINE_TASK(TestTask6)
  {
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(6);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask6);

  SF_DEFINE_TASK(TestTask7)
  {
    SF_ENQUEUE_JOB(TestTask6, nullptr);
    SF_YEILD_AND_WAIT();
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(7);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask7);

  SF_DEFINE_TASK(TestTask8)
  {
    SF_ENQUEUE_JOB(TestTask7, nullptr);
    SF_YEILD_AND_WAIT();
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(8);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask8);

  SF_DEFINE_TASK(TestTask9)
  {
    SF_ENQUEUE_JOB(TestTask8, nullptr);
    SF_YEILD_AND_WAIT();
    testMutex.lock();
    counter++;
    taskNumCompletion.push_back(9);
    testMutex.unlock();
  } SF_END_DEFINE_TASK(TestTask9);
}
#pragma optimize( "", on ) 
namespace UnitTests
{
  TEST_CLASS(utTaskManager)
  {
    TEST_METHOD(TaskManager1)
    {
      TaskManager *man = TaskManager::Instance();

      man->AddNode("TestTask1");
      man->SetStartingTask("TestTask1");
      man->AddNode("TestTask2");
      man->AddNode("TestTask3");
      man->AddNode("TestTask4");

      man->SetDependency("TestTask2", "TestTask1");
      man->SetDependency("TestTask3", "TestTask1");
      man->SetDependency("TestTask4", "TestTask1");

      man->AddNode("TestTask5");

      man->SetDependency("TestTask5", "TestTask2");
      man->SetDependency("TestTask5", "TestTask3");
      man->SetDependency("TestTask5", "TestTask4");

      man->CompleteGraph();

      man->RunTasks();

      Assert::IsTrue(counter == 5);

      for (int i = 1; i <= taskNumCompletion.size(); ++i)
      {
        Assert::IsTrue(taskNumCompletion[i - 1] == i);
      }
      counter = 0;
      taskNumCompletion.clear();
    }

    TEST_METHOD(TaskManager2)
    {
      TaskManager *man = TaskManager::Instance();

      man->AddNode("TestTask9");
      man->SetStartingTask("TestTask9");
      man->CompleteGraph();

      man->RunTasks();

      Assert::IsTrue(counter == 4);

      int i = 6;
      for (int j = 0; i < taskNumCompletion.size(); ++j)
      {
        Assert::IsTrue(taskNumCompletion[j] == i++);
      }
      counter = 0;
      taskNumCompletion.clear();
    }
  };
}
