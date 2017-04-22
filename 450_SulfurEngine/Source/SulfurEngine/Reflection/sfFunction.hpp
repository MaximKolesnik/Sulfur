/******************************************************************************/
/*!
\par     Sulfur
\file    sfFunction.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Function bindings

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace details
{
template<int N> struct placeholder {};
}

namespace std
{
template<int N> struct is_placeholder< details::placeholder<N> > : std::integral_constant<int, N> {};
}

namespace details
{

template<int N>
struct easy_binder
{
  template <typename Object, typename ReturnType, typename... Args>
  static std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*MemPtr)(Args...), Object* obj)
  {
    return easy_binder<N - 1>::easy_bind(MemPtr, obj, placeholder<N>());
  }

  template <typename Object, typename ReturnType, typename... Args, typename... PlaceHolders>
  static std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*MemPtr)(Args...), Object* obj, PlaceHolders... placeholders)
  {
    return easy_binder<N - 1>::easy_bind(MemPtr, obj, placeholder<N>(), placeholders...);
  }
};

template<>
struct easy_binder<0>
{
  template <typename Object, typename ReturnType, typename... Args, typename... PlaceHolders>
  static std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*MemPtr)(Args...), Object* obj, PlaceHolders... placeholders)
  {
    return std::function<ReturnType(Args...)>(std::bind(MemPtr, obj, placeholders...));
  }
};

}

template <typename Object, typename ReturnType, typename... Args>
std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*MemPtr)(Args...), Object* obj)
{
  return details::easy_binder<sizeof...(Args)>::easy_bind(MemPtr, obj);
}

class Function
{

public:
  template <typename ReturnType, typename...Args>
  ReturnType Call(Args...args)
  {
    ReturnType returnValue;
    CallReturnImpl(&returnValue);
    return returnValue;
  }

  template <typename...Args>
  void Call(Args...args)
  {
    CallImpl();
  }

  template <typename ReturnType, typename Arg1>
  ReturnType Call(Arg1 arg)
  {
    ReturnType returnValue;
    CallReturnImpl(&returnValue);
    return returnValue;
  }

protected:
  virtual void CallImpl() = 0;
  virtual void CallReturnImpl(void *returnValue) = 0;
  virtual void CallReturnImpl(void *returnValue, void *arg1) = 0;

};

template <typename ClassType, typename ReturnType, typename...Args>
class BoundMemberFunction : public Function
{

public:
  typedef ReturnType(ClassType::*FunctionPtr)(Args...);
  typedef std::function<ReturnType(Args...)> FunctionType;

public:
  BoundMemberFunction(FunctionPtr function, ClassType *classInstance)
  {
    m_function = easy_bind<ClassType, ReturnType, Args...>(function, classInstance);
  }

protected:
  virtual void CallImpl() override
  {
    m_function();
  }

  virtual void CallReturnImpl(void *returnValue) override
  {
    *reinterpret_cast<ReturnType*>(returnValue) = m_function();
  }

  virtual void CallReturnImpl(void *returnValue, void *arg1) override
  {
    *reinterpret_cast<ReturnType*>(returnValue) = m_function(*reinterpret_cast<int*>(arg1));
  }

private:
  FunctionType m_function;

};
