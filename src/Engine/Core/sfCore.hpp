#pragma once

namespace Sulfur
{
  class Core
  {
  public:
    static Core* Instance(void)
    {
      if (!m_instance)
        m_instance = new Core();

      return m_instance;
    }

    void StartUp(int argc, char **argv);
    void GameLoop(void);
    void ShutDown(void);

  private:
    Core(void) {};
    ~Core(void) {};

    Core(const Core&) = delete;
    Core& operator=(const Core&) = delete;

    static Core* m_instance;
  };
}
