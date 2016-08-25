#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

#include "Core/sfCore.hpp"
#include "Math\sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math\sfMatrix3.hpp"
#include "Math\sfMatrix4.hpp"


void test(void)
{
  int i = 0;
  while(true)
    std::cout << i++ << std::endl;
}
#pragma optimize("", off)
int main(int argc, char** argv)
{
  Sulfur::Matrix4 m(1, 3, 2, 5,
    4, 5, 7, 9,
    10, 15, 5, 4,
    47, 65, 13, 8);
    
  float *mf = new float[16];
  mf[0] = 1, mf[1] = 3, mf[2] = 2, mf[3] = 5,
    mf[4] = 4, mf[5] = 5, mf[6] = 7, mf[7] = 9,
    mf[8] = 10, mf[9] = 15, mf[10] = 5, mf[11] = 4,
    mf[12] = 47, mf[13] = 65, mf[14] = 13, mf[15] = 8;


  Sulfur::Matrix3 m1(5, 7, 9,
    15, 5, 4,
    65, 13, 8);

  Sulfur::Vector3 v(1, 2, 3);
  Sulfur::Quaternion q(1, 0, 0, 0);


  /*auto start2 = std::chrono::steady_clock::now();
  for (int i = 0; i < 100000000; ++i)
  {
    m.SetRotation(q);
  }
  auto end2 = std::chrono::steady_clock::now();
  auto timeSimd = std::chrono::duration<double, std::milli>(end2 - start2).count();

  std::cout << "SIMD time: " << timeSimd << "ms" << std::endl;*/

  /*Sulfur::Core *core = Sulfur::Core::GetInstance();

  core->StartUp(argc, argv);
  core->GameLoop();
  core->ShutDown();*/


  system("pause");
}
#pragma optimize( "", on ) 