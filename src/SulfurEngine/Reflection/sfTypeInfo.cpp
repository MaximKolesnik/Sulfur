/******************************************************************************/
/*!
\par     Sulfur
\file    sfTypeInfo.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Reflection type information

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfTypeInfo.hpp"
#include "Math/sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfQuaternion.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"

namespace Sulfur
{

SF_REGISTER_BASE_TYPE(bool)
SF_REGISTER_BASE_TYPE(UINT8)
SF_REGISTER_BASE_TYPE(UINT16)
SF_REGISTER_BASE_TYPE(UINT32)
SF_REGISTER_BASE_TYPE(UINT64)
SF_REGISTER_BASE_TYPE(INT8)
SF_REGISTER_BASE_TYPE(INT16)
SF_REGISTER_BASE_TYPE(INT32)
SF_REGISTER_BASE_TYPE(INT64)
SF_REGISTER_BASE_TYPE(Real)
SF_REGISTER_BASE_TYPE(std::string)

SF_REGISTER_BASE_TYPE(Vector2)
SF_REGISTER_BASE_TYPE(Vector3)
SF_REGISTER_BASE_TYPE(Vector4)
SF_REGISTER_BASE_TYPE(Quaternion)
SF_REGISTER_BASE_TYPE(Matrix3)
SF_REGISTER_BASE_TYPE(Matrix4)

}
