/******************************************************************************/
/*!
\par     Sulfur
\file    sfSingleton.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Singleton helper macros

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#define PRIVATE_CTOR_DTOR(className) \
private: \
className(); \
~className();

#define PRIVATE_CTOR_DTOR_EMPTY(className) \
private: \
className() {} \
~className() {}

#define FORBID_COPY(className) \
private: \
className(const className&) = delete; \
className& operator=(const className&) = delete;

#define SINGLETON_INSTANCE(className) \
public: \
static className* Instance(void) { \
  static className instance; \
  return &instance; \
}                    

#define SINGLETON(className) \
SINGLETON_INSTANCE(className) \
FORBID_COPY(className) \
PRIVATE_CTOR_DTOR(className)