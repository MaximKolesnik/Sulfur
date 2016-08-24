#pragma once

#include <assert.h>

namespace Sulfur
{
  template <typename T>
  class Singleton
  {
  public:
    static T* GetInstance(void)
    {
      if (!m_instance)
        m_instance = new T;

      return m_instance;
    }

  private:
    Singleton(const Singleton &copy) = delete;
    Singleton& operator=(const Singleton &copy) = delete;

  protected:
    static T* m_instance;

    Singleton(void)
    {
      assert(!m_instance);
      m_instance = static_cast <T*>(this);
    }

    virtual ~Singleton(void)
    {
    }
  };

  template <typename T>
  typename T* Singleton<T>::m_instance = nullptr;
}
