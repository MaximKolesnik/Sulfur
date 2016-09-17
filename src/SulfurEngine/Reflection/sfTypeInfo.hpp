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
#pragma once

#define SF_REGISTER_TYPE(type, base, ...) \
template <> const TypeInfo Sulfur::TypeInfoRegistry<type>::s_typeInfo = TypeInfo(#type, TypeInfo::IdGen(), base, {__VA_ARGS__}); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::vector<type>>::s_typeInfo = TypeInfo("std::vector<"#type">", TypeInfo::IdGen(), nullptr, {}); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::list<type>>::s_typeInfo = TypeInfo("std::list<"#type">", TypeInfo::IdGen(), nullptr, {}); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::unordered_map<std::string, type>>::s_typeInfo = TypeInfo("std::unordered_map<std::string, "#type">", TypeInfo::IdGen(), nullptr, {});

#define SF_REGISTER_ENUM_TYPE(type, values, ...) SF_REGISTER_TYPE(type, nullptr, values, ##__VA_ARGS__)
#define SF_REGISTER_BASE_TYPE(type) SF_REGISTER_TYPE(type, nullptr, {})
#define SF_REGISTER_REFLECTED_TYPE(type) SF_REGISTER_TYPE(type, nullptr, {})
#define SF_TYPE_INFO(type) (&Sulfur::TypeInfoRegistry<type>::s_typeInfo)
#define SF_TYPE_ID(type) (SF_TYPE_INFO(type)->GetId())

namespace Sulfur
{
	
  class TypeInfo
  {
  
  public:
    TypeInfo(const std::string& name, UINT32 id, const TypeInfo *parentClass, const std::vector<std::string>& enumValues)
      : m_name(name), m_id(id),  m_parentClass(parentClass), m_enumValues(enumValues)
    {
    }

    const std::string& GetName() const { return m_name; }
    UINT32 GetId() const { return m_id; }
    bool IsBaseType() const { return m_parentClass == nullptr; }
    const TypeInfo *GetParentClass() const { return m_parentClass; }
    const std::vector<std::string>& GetEnumValues() const { return m_enumValues; }

    template <typename Type>
    bool IsDerivedFrom() const
    {
      if (this == &TypeInfoRegistry<Type>::s_typeInfo)
        return true;
      if (m_parentClass != nullptr)
        return m_parentClass->IsDerivedFrom<Type>();
      return false;
    }

    template <typename Type>
    friend class TypeInfoRegistry;

  private:
    static UINT32 IdGen()
    {
      static UINT32 id = 0;
      return id++;
    }

  private:
    std::string m_name;
    UINT32 m_id;
    const TypeInfo *m_parentClass;
    std::vector<std::string> m_enumValues;

  };

  template <typename Type>
  class TypeInfoRegistry
  {

  public:
    static const TypeInfo s_typeInfo;

  };
  
}