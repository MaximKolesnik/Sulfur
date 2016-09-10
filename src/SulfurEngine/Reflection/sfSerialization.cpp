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
#include "Math/sfMatrix4.hpp"

namespace Sulfur
{

template <>
void Serialization::Serialize<ReflectionBase>(std::ostream& str, const ReflectionBase& value)
{
  ReflectionBase::PropertyList properties;
  value.GetProperties(properties);

  for (Property* prop : properties)
    prop->Serialize(str, &value);
}

template <>
void Serialization::Deserialize<ReflectionBase>(std::istream& str, ReflectionBase& value)
{
  ReflectionBase::PropertyList properties;
  value.GetProperties(properties);

  for (Property* prop : properties)
    prop->Deserialize(str, &value);
}

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

template <>
void Serialization::Serialize<Vector2>(std::ostream& str, const Vector2& value)
{
  str.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template <>
void Serialization::Deserialize<Vector2>(std::istream& str, Vector2& value)
{
  str.read(reinterpret_cast<char *>(&value), sizeof(value));
}

template <>
void Serialization::Serialize<Vector3>(std::ostream& str, const Vector3& value)
{
  str.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template <>
void Serialization::Deserialize<Vector3>(std::istream& str, Vector3& value)
{
  str.read(reinterpret_cast<char *>(&value), sizeof(value));
}

template <>
void Serialization::Serialize<Vector4>(std::ostream& str, const Vector4& value)
{
  str.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template <>
void Serialization::Deserialize<Vector4>(std::istream& str, Vector4& value)
{
  str.read(reinterpret_cast<char *>(&value), sizeof(value));
}

template <>
void Serialization::Serialize<Quaternion>(std::ostream& str, const Quaternion& value)
{
  str.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template <>
void Serialization::Deserialize<Quaternion>(std::istream& str, Quaternion& value)
{
  str.read(reinterpret_cast<char *>(&value), sizeof(value));
}

template <>
void Serialization::Serialize<Matrix4>(std::ostream& str, const Matrix4& value)
{
}

template <>
void Serialization::Deserialize<Matrix4>(std::istream& str, Matrix4& value)
{
}

}
