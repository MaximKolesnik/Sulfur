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
