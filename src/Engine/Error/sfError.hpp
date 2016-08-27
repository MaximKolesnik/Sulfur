/******************************************************************************/
/*!
\par     Sulfur
\file    Error.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/24/2016

\brief   Error and warning handlers.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../Logger/sfLogger.hpp"
#include <Winuser.h>

/*******************************************************************************
 Maxim Kolesnik: TODO: Block all threads when MessageBox is displayed?
*******************************************************************************/

#ifdef _DEBUG
#define SF_WARNING(message) \
Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "WARNING")

#define SF_WARNING_EXP(exp, message) \
if (!(exp)) Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "WARNING")

#define SF_WARNING_MB(message) \
{ \
Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "WARNING"); \
std::string dialogText = message; \
dialogText += "\n"; \
dialogText += "Do you want to continue? No to start debugging"; \
int res = MessageBox(NULL, dialogText.c_str(), "Warning", MB_APPLMODAL | MB_YESNOCANCEL); \
switch (res) \
{ \
  case IDCANCEL: \
  exit(EXIT_FAILURE); \
  break; \
  case IDNO: \
  DebugBreak(); \
  break;  \
} \
}

#define SF_WARNING_EXP_MB(exp, message) \
if (!(exp)) \
{ \
Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "WARNING"); \
std::string dialogText = message; \
dialogText += "\n"; \
dialogText += "Do you want to continue? No to start debugging"; \
int res = MessageBox(NULL, dialogText.c_str(), "Warning", MB_APPLMODAL | MB_YESNOCANCEL); \
switch (res) \
{ \
  case IDCANCEL: \
  exit(EXIT_FAILURE); \
  break; \
  case IDNO: \
  DebugBreak(); \
  break;  \
}\
}

#define SF_ASSERT(exp, message) \
if (!(exp)) \
{ \
  Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "ASSERT"); \
  assert(false && message); \
}

#else
#define SF_WARNING(message)
#define SF_WARNING_EXP(exp, message)
#define SF_WARNING_MB(message)
#define SF_WARNING_EXP_MB(exp, message)
#define SF_ASSERT(exp, message)
#endif

//Use critical error without logging, if not sure that logger is initialized
#define SF_LOG_CRITICAL_ERR(message) \
{ \
Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "CRITICAL ERROR"); \
std::string dialogText = message; \
dialogText += "\n"; \
dialogText += "CRITICAL ERROR"; \
int res = MessageBox(NULL, dialogText.c_str(), "CRITICAL ERROR", MB_APPLMODAL | MB_OK); \
switch (res) \
{ \
  case IDOK: \
  exit(EXIT_FAILURE); \
} \
}

#define SF_CRITICAL_ERR(message) \
{ \
std::string dialogText = message; \
dialogText += "\n"; \
dialogText += "CRITICAL ERROR"; \
int res = MessageBox(NULL, dialogText.c_str(), "CRITICAL ERROR", MB_APPLMODAL | MB_OK); \
switch (res) \
{ \
  case IDOK: \
  exit(EXIT_FAILURE); \
} \
}

#define SF_CRITICAL_ERR_EXP(exp, message) \
if (!(exp)) { \
std::string dialogText = message; \
dialogText += "\n"; \
dialogText += "CRITICAL ERROR"; \
int res = MessageBox(NULL, dialogText.c_str(), "CRITICAL ERROR", MB_APPLMODAL | MB_OK); \
switch (res) \
{ \
  case IDOK: \
  exit(EXIT_FAILURE); \
} \
}