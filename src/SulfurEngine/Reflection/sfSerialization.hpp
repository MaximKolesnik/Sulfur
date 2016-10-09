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

#include <unordered_set>

#define SF_BASE_TYPE_SPECIALIZATION(type) \
template <> UINT32 Serialization::SerializedSize<type>(const type& value); \
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value); \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value);

#define SF_BASE_TYPE_SPECIALIZATION_FD(type) \
class type; \
template <> UINT32 Serialization::SerializedSize<type>(const type& value); \
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value); \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value);

#define SF_BASE_TYPE_SERIALIZATION_DEFAULT(type)\
template <> UINT32 Serialization::SerializedSize<type>(const type& value) { return sizeof(value); } \
template <> void Serialization::Serialize<type>(std::ostream& str, const type& value) { str.write(reinterpret_cast<const char*>(&value), sizeof(value)); } \
template <> void Serialization::Deserialize<type>(std::istream& str, type& value) { str.read(reinterpret_cast<char *>(&value), sizeof(value)); }

namespace Sulfur
{

  class ReflectionBase;
  template <typename T> class SlotMap;

  class Serialization
  {
  
  public:
    template <typename T>
    static UINT32 SerializedSize(const T& value)
    {
      static_assert(std::is_base_of<ReflectionBase, T>::value, "Base type serialization not implemented");

      UINT32 size = 0;
      ReflectionBase::PropertyList properties;
      value.GetProperties(properties);

      for (Property* prop : properties)
      {
        std::string name = prop->GetName();
        size += SerializedSize(name);
        size += prop->SerializedSize(&value);
      }

      return size;
    }

    template <typename T>
    static UINT32 SerializedSize(const std::vector<T>& value)
    {
      UINT32 size = 0;
      UINT32 count = (UINT32)value.size();
      for (UINT32 i = 0; i < size; ++i)
        size += SerializedSize(value[i]);

      return size;
    }

    template <typename T>
    static UINT32 SerializedSize(const std::list<T>& value)
    {
      UINT32 size = 0;
      for (auto it = value.begin(); it != value.end(); ++it)
        size += SerializedSize(*it);

      return size;
    }

    template <typename Kt, typename Vt>
    static UINT32 SerializedSize(const std::unordered_map<Kt, Vt>& value)
    {
      UINT32 size = 0;
      for (auto it = value.begin(); it != value.end(); ++it)
      {
        size += SerializedSize(it->first);
        size += SerializedSize(it->second);
      }

      return size;
    }

    template <typename T>
    static UINT32 SerializedSize(const std::unordered_set<T>& value)
    {
      UINT32 size = 0;
      for (auto it = value.begin(); it != value.end(); ++it)
        size += SerializedSize(*it);

      return size;
    }

    template <typename T>
    static UINT32 SerializedSize(const SlotMap<T>& value)
    {
      UINT32 size = 0;
      for (auto it = value.begin(); it != value.end(); ++it)
      {
        size += SerializedSize(value.GetSize());
        size += SerializedSize(*static_cast<T*>(*it));
      }

      return size;
    }

    template <typename T>
    static void Serialize(std::ostream& str, const T& value)
    {
      static_assert(std::is_base_of<ReflectionBase, T>::value, "Base type serialization not implemented");

      ReflectionBase::PropertyList properties;
      value.GetProperties(properties);

      Serialize(str, (UINT32)properties.size());
      for (Property* prop : properties)
      {
        Serialize(str, prop->GetName());
        Serialize(str, prop->SerializedSize(&value));
        prop->Serialize(str, &value);
      }
    }

    template <typename T>
    static void Serialize(std::ostream& str, const std::vector<T>& value)
    {
      UINT32 size = (UINT32)value.size();
      Serialize(str, size);

      for (UINT32 i = 0; i < size; ++i)
        Serialize(str, value[i]);
    }

    template <typename T>
    static void Serialize(std::ostream& str, const std::list<T>& value)
    {
      UINT32 size = (UINT32)value.size();
      Serialize(str, size);

      for (auto it = value.begin(); it != value.end(); ++it)
        Serialize(str, *it);
    }

    template <typename Kt, typename Vt>
    static void Serialize(std::ostream& str, const std::unordered_map<Kt, Vt>& value)
    {
      UINT32 size = (UINT32)value.size();
      Serialize(str, size);

      for (auto it = value.begin(); it != value.end(); ++it)
      {
        Serialize(str, it->first);
        Serialize(str, it->second);
      }
    }

    template <typename T>
    static void Serialize(std::ostream& str, const std::unordered_set<T>& value)
    {
      UINT32 size = (UINT32)value.size();
      Serialize(str, size);

      for (auto it = value.begin(); it != value.end(); ++it)
      {
        Serialize(str, *it);
      }
    }

    template <typename T>
    static void Serialize(std::ostream& str, const SlotMap<T>& value)
    {
      UINT32 size = value.GetSize();
      Serialize(str, size);

      for (auto it = value.begin(); it != value.end(); ++it)
      {
        Serialize(str, it->GetHndl());
        Serialize(str, *static_cast<const T*>(*it));
      }
    }

    template <typename T>
    static void Deserialize(std::istream& str, T& value)
    {
      static_assert(std::is_base_of<ReflectionBase, T>::value, "Base type serialization not implemented");

      ReflectionBase::PropertyList properties;
      value.GetProperties(properties);

      UINT32 propertyCount = 0;
      Deserialize(str, propertyCount);

      for (UINT32 i = 0; i < propertyCount; ++i)
      {
        std::string name;
        Deserialize(str, name);

        UINT32 serializedSize = 0;
        Deserialize(str, serializedSize);

        Property *prop = value.GetProperty(name);

        // Skip properties that weren't serialized
        if (prop == nullptr)
        {
          str.seekg(serializedSize, std::ios_base::cur);
          continue;
        }

        prop->Deserialize(str, &value);
      }
    }

    template <typename T>
    static void Deserialize(std::istream& str, std::vector<T>& value)
    {
      UINT32 size;
      Deserialize(str, size);
      value.resize(size);

      for (UINT32 i = 0; i < size; ++i)
        Deserialize(str, value[i]);
    }

    template <typename T>
    static void Deserialize(std::istream& str, std::list<T>& value)
    {
      UINT32 size;
      Deserialize(str, size);

      for (UINT32 i = 0; i < size; ++i)
      {
        value.push_back(T());
        Deserialize(str, value.back());
      }
    }

    template <typename Kt, typename Vt>
    static void Deserialize(std::istream& str, std::unordered_map<Kt, Vt>& value)
    {
      UINT32 size;
      Deserialize(str, size);

      for (UINT32 i = 0; i < size; ++i)
      {
        Kt key;
        Deserialize(str, key);

        value[key] = Vt();
        Deserialize(str, value[key]);
      }
    }

    template <typename T>
    static void Deserialize(std::istream& str, std::unordered_set<T>& value)
    {
      UINT32 size;
      Deserialize(str, size);

      for (UINT32 i = 0; i < size; ++i)
      {
        T val;
        Deserialize(str, val);
        value.insert(val);
      }
    }

    template <typename T>
    static void Deserialize(std::istream& str, SlotMap<T>& value)
    {
      UINT32 size;
      Deserialize(str, size);

      for (UINT32 i = 0; i < size; ++i)
      {
        HNDL handle;
        Deserialize(str, handle);

        T val;
        Deserialize(str, val);

        value.CreateAt(handle);
        IEntity *element = value.At(handle);
        Deserialize(str, *element);
      }
    }

  };


  SF_BASE_TYPE_SPECIALIZATION(bool)
  SF_BASE_TYPE_SPECIALIZATION(UINT8)
  SF_BASE_TYPE_SPECIALIZATION(UINT16)
  SF_BASE_TYPE_SPECIALIZATION(UINT32)
  SF_BASE_TYPE_SPECIALIZATION(UINT64)
  SF_BASE_TYPE_SPECIALIZATION(INT8)
  SF_BASE_TYPE_SPECIALIZATION(INT16)
  SF_BASE_TYPE_SPECIALIZATION(INT32)
  SF_BASE_TYPE_SPECIALIZATION(INT64)
  SF_BASE_TYPE_SPECIALIZATION(Real)
  SF_BASE_TYPE_SPECIALIZATION(std::string)

  SF_BASE_TYPE_SPECIALIZATION_FD(Vector2)
  SF_BASE_TYPE_SPECIALIZATION_FD(Vector3)
  SF_BASE_TYPE_SPECIALIZATION_FD(Vector4)
  SF_BASE_TYPE_SPECIALIZATION_FD(Quaternion)
  SF_BASE_TYPE_SPECIALIZATION_FD(Matrix4)
  SF_BASE_TYPE_SPECIALIZATION_FD(Vertex)
  SF_BASE_TYPE_SPECIALIZATION_FD(Color)
  SF_BASE_TYPE_SPECIALIZATION_FD(ResourcePath)
  
}