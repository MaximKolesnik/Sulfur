/******************************************************************************/
/*!
\par     Sulfur
\file    sfSerialization.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/7/2016

\brief   Serialization functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#define BASE_TYPE_SPECIALIZATION(type) \
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value); \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value);

#define BASE_TYPE_SPECIALIZATION_FD(type) \
class type; \
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value); \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value);

#define BASE_TYPE_SERIALIZATION_DEFAULT(type)\
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value) { str.write(reinterpret_cast<const char*>(&value), sizeof(value)); } \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value) { str.read(reinterpret_cast<char *>(&value), sizeof(value)); }

namespace Sulfur
{

  class ReflectionBase;

  class Serialization
  {
  
  public:
    template <typename T>
    static void Serialize(std::ostream& str, const T& value)
    {
      static_assert(std::is_base_of<ReflectionBase, T>::value, "Base type serialization not implemented");

      ReflectionBase::PropertyList properties;
      value.GetProperties(properties);

      for (Property* prop : properties)
        prop->Serialize(str, &value);
    }

    template <typename T>
    static void Deserialize(std::istream& str, T& value)
    {
      static_assert(std::is_base_of<ReflectionBase, T>::value, "Base type serialization not implemented");

      ReflectionBase::PropertyList properties;
      value.GetProperties(properties);

      for (Property* prop : properties)
        prop->Deserialize(str, &value);
    }

  };

  BASE_TYPE_SPECIALIZATION(std::string)
  BASE_TYPE_SPECIALIZATION_FD(Vector2)
  BASE_TYPE_SPECIALIZATION_FD(Vector3)
  BASE_TYPE_SPECIALIZATION_FD(Vector4)
  BASE_TYPE_SPECIALIZATION_FD(Quaternion)
  BASE_TYPE_SPECIALIZATION_FD(Matrix4)
  
}