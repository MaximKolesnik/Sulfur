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

#define SF_PRIVATE_CTOR_DTOR(className) \
private: \
className(); \
~className();

#define SF_PRIVATE_CTOR_DTOR_EMPTY(className) \
private: \
className() {} \
~className() {}

#define SF_FORBID_COPY(className) \
private: \
className(const className&) = delete; \
className& operator=(const className&) = delete;

#define SF_SINGLETON_INSTANCE(className) \
public: \
static className* Instance(void) { \
  static className instance; \
  return &instance; \
}                    

#define SF_SINGLETON(className) \
SF_SINGLETON_INSTANCE(className) \
SF_FORBID_COPY(className) \
SF_PRIVATE_CTOR_DTOR(className)
