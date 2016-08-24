#pragma once

namespace Sulfur
{
  template <typename T>
  class Singleton
  {
  public:
    static T* GetInstance(void)
    {
      static T instance;

      return &instance;
    }

    Singleton(const Singleton &copy) = delete;
    Singleton& operator=(const Singleton &copy) = delete;
  protected:
    Singleton(void)
    {
    }

    ~Singleton(void)
    {
    }
  };
}
