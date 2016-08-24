#pragma once

#include "../Types/sfSingleton.hpp"

namespace Sulfur
{
  class Core : public Singleton<Core>
  {
  public:
    void StartUp(int argc, char **argv);
    void GameLoop(void);
    void ShutDown(void);
  private:
  };
}
