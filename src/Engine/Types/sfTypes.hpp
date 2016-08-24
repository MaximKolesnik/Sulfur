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
  typedef uint8_t  UINT8;
  typedef uint16_t UINT16;
  typedef uint32_t UINT32;
  typedef uint64_t UINT64;

  typedef UINT64 hndl;
}