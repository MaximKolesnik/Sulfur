/******************************************************************************/
/*!
\par     Sulfur
\file    sfSerialization.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/7/2016

\brief   Serialization functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSerialization.hpp"
#include "sfReflection.hpp"
#include "Math/sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfQuaternion.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"

namespace Sulfur
{

template <>
void Serialization::Serialize<std::string>(std::ostream& str, const std::string& value)
{
  UINT32 size = (UINT32)value.size();
  str.write(reinterpret_cast<const char*>(&size), sizeof(size));
  str.write(value.data(), size);
}

template <>
void Serialization::Deserialize<std::string>(std::istream& str, std::string& value)
{
  UINT32 size;
  str.read(reinterpret_cast<char*>(&size), sizeof(size));

  char *buffer = new char[size + 1];
  buffer[size] = 0;
  str.read(buffer, size);
  value = buffer;

  delete[] buffer;
}

BASE_TYPE_SERIALIZATION_DEFAULT(UINT8)
BASE_TYPE_SERIALIZATION_DEFAULT(UINT16)
BASE_TYPE_SERIALIZATION_DEFAULT(UINT32)
BASE_TYPE_SERIALIZATION_DEFAULT(UINT64)
BASE_TYPE_SERIALIZATION_DEFAULT(INT8)
BASE_TYPE_SERIALIZATION_DEFAULT(INT16)
BASE_TYPE_SERIALIZATION_DEFAULT(INT32)
BASE_TYPE_SERIALIZATION_DEFAULT(INT64)
BASE_TYPE_SERIALIZATION_DEFAULT(Real)

BASE_TYPE_SERIALIZATION_DEFAULT(Vector2)
BASE_TYPE_SERIALIZATION_DEFAULT(Vector3)
BASE_TYPE_SERIALIZATION_DEFAULT(Vector4)
BASE_TYPE_SERIALIZATION_DEFAULT(Quaternion)
BASE_TYPE_SERIALIZATION_DEFAULT(Matrix3)
BASE_TYPE_SERIALIZATION_DEFAULT(Matrix4)

}
