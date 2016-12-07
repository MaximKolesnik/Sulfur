/******************************************************************************/
/*!
\par     Sulfur
\file    sfTypes.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/5/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <stdint.h>

namespace Sulfur
{
#ifndef SF_USE_DOUBLE_PRECISION

  typedef float Real;

#else

  typedef double Real;

#endif //SF_USE_DOUBLE_PRECISION


  typedef int8_t   INT8;
  typedef int16_t  INT16;
  typedef int32_t  INT32;
  typedef int64_t  INT64;
  typedef uint8_t  BYTE;
  typedef uint8_t  UINT8;
  typedef uint16_t UINT16;
  typedef uint32_t UINT32;
  typedef uint64_t UINT64;

  //Handles
  typedef UINT64 HNDL;
#define SF_INV_HANDLE UINT64_MAX
}