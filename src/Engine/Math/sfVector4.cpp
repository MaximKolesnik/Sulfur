#include "sfVector4.hpp"

namespace Sulfur
{
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_zero = Vector4(0.0, 0.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_xAxis = Vector4(1.0, 0.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_yAxis = Vector4(0.0, 1.0, 0.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_zAxis = Vector4(0.0, 0.0, 1.0, 0.0);
  const Vector4 SF_ATTR_ALIGN_16 Vector4::c_wAxis = Vector4(0.0, 0.0, 0.0, 1.0);
}