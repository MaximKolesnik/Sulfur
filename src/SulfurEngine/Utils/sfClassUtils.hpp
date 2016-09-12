/******************************************************************************/
/*!
\par     Sulfur
\file    sfClassUtils.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Class helper defines

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Reflection/sfFunction.hpp"

#define SF_CALLBACK(name, ...)                                                                      \
public:                                                                                             \
template <typename ClassType> using name##CallbackMemberFnPtr = void(ClassType::*)(##__VA_ARGS__);  \
typedef void(*name##CallbackStaticFnPtr)(##__VA_ARGS__);                                            \
typedef std::function<void(##__VA_ARGS__)> name##CallbackFn;                                        \
template <typename ClassType>                                                                       \
void RegisterCallback##name(ClassType* instance, name##CallbackMemberFnPtr<ClassType> function)     \
{                                                                                                   \
  m_callbacks##name.push_back(easy_bind(function, instance));                                       \
}                                                                                                   \
void RegisterCallback##name(name##CallbackStaticFnPtr function)                                     \
{                                                                                                   \
  m_callbacks##name.push_back(std::function<void(##__VA_ARGS__)>(function));                        \
}                                                                                                   \
private:                                                                                            \
std::vector<name##CallbackFn> m_callbacks##name;                                                    \
template <typename...Args>                                                                          \
void Notify##name(Args...args) const                                                                \
{                                                                                                   \
  for (const name##CallbackFn& function : m_callbacks##name)                                        \
    function(args...);                                                                              \
}