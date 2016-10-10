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
#include "Math/sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfQuaternion.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Modules/Graphics/Types/sfColor.hpp"
#include "Modules/Resource/sfResourcePath.hpp"
#include "DataStructures/sfSlotMap.hpp"

namespace Sulfur
{

template <>
UINT32 Serialization::SerializedSize<std::string>(const std::string& value)
{
  UINT32 size = (UINT32)value.size();
  return (UINT32)value.size() + SerializedSize(size);
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
UINT32 Serialization::SerializedSize<ISlotMap>(const ISlotMap& value)
{
  UINT32 size = 0;
  for (auto it = value.begin(); it != value.end(); ++it)
  {
    size += SerializedSize(value.GetSize());
    size += SerializedSize(**it);
  }

  return size;
}

template <>
void Serialization::Serialize<ISlotMap>(std::ostream& str, const ISlotMap& value)
{
  UINT32 size = value.GetSize();
  Serialize(str, size);

  for (auto it = value.begin(); it != value.end(); ++it)
  {
    Serialize(str, it->GetHndl());
    Serialize(str, **it);
  }
}

template <>
void Serialization::Deserialize<ISlotMap>(std::istream& str, ISlotMap& value)
{
  UINT32 size;
  Deserialize(str, size);

  for (UINT32 i = 0; i < size; ++i)
  {
    HNDL handle;
    Deserialize(str, handle);

    value.CreateAt(handle);
    IEntity *element = value.At(handle);
    Deserialize(str, *element);
  }
}

template <>
UINT32 Serialization::SerializedSize<ResourcePath>(const ResourcePath& value)
{
  UINT32 size = (UINT32)value.size();
  return (UINT32)value.size() + SerializedSize(size);
}

template <>
void Serialization::Serialize<ResourcePath>(std::ostream& str, const ResourcePath& value)
{
  UINT32 size = (UINT32)value.size();
  str.write(reinterpret_cast<const char*>(&size), sizeof(size));
  str.write(value.data(), size);
}

template <>
void Serialization::Deserialize<ResourcePath>(std::istream& str, ResourcePath& value)
{
  UINT32 size;
  str.read(reinterpret_cast<char*>(&size), sizeof(size));

  char *buffer = new char[size + 1];
  buffer[size] = 0;
  str.read(buffer, size);
  value = buffer;

  delete[] buffer;
}

SF_BASE_TYPE_SERIALIZATION_DEFAULT(bool)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(UINT8)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(UINT16)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(UINT32)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(UINT64)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(INT8)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(INT16)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(INT32)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(INT64)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Real)

SF_BASE_TYPE_SERIALIZATION_DEFAULT(Vector2)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Vector3)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Vector4)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Quaternion)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Matrix3)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Matrix4)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Vertex)
SF_BASE_TYPE_SERIALIZATION_DEFAULT(Color)

}
