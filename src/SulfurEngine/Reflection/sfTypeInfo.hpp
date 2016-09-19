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
#include "Types/sfSingleton.hpp"

#define SF_TYPE_INFO(type) (&Sulfur::TypeInfoRegistry<type>::s_typeInfo)
#define SF_TYPE_ID(type) (SF_TYPE_INFO(type)->GetId())

#define SF_REGISTER_TYPE(type, base, ...) \
template <> const TypeInfo Sulfur::TypeInfoRegistry<type>::s_typeInfo(#type, TypeInfo::IdGen(), base, {__VA_ARGS__}, {}); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::vector<type>>::s_typeInfo("std::vector", TypeInfo::IdGen(), nullptr, {}, { SF_TYPE_INFO(type) }); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::list<type>>::s_typeInfo("std::list", TypeInfo::IdGen(), nullptr, {}, { SF_TYPE_INFO(type) }); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::unordered_map<std::string, type>>::s_typeInfo("std::unordered_map", TypeInfo::IdGen(), nullptr, {}, { SF_TYPE_INFO(std::string), SF_TYPE_INFO(type) });

#define SF_REGISTER_ENUM_TYPE(type, values, ...) SF_REGISTER_TYPE(type, nullptr, values, ##__VA_ARGS__)
#define SF_REGISTER_BASE_TYPE(type) SF_REGISTER_TYPE(type, nullptr)
#define SF_REGISTER_REFLECTED_TYPE(type, baseType) SF_REGISTER_TYPE(type, SF_TYPE_INFO(baseType))

namespace Sulfur
{
	
enum ContainerType
{
  NONE,
  VECTOR,
  LIST,
  MAP
};

class TypeInfo
{
  SF_FORBID_COPY(TypeInfo)
  
public:
  typedef std::vector<std::string> EnumValuesList;
  typedef std::vector<const TypeInfo*> TypesList;
  template <typename Type> friend class TypeInfoRegistry;

private:
  TypeInfo(const std::string& name, UINT32 id, const TypeInfo *parentClass, const EnumValuesList& enumValues, const TypesList& templateTypes)
    : m_containerType(NONE), m_name(name), m_id(id),  m_parentClass(parentClass), m_enumValues(enumValues), m_templateTypes(templateTypes)
  {
    if (m_name == "std::vector") m_containerType = VECTOR;
    else if(m_name == "std::list") m_containerType = LIST;
    else if(m_name == "std::unordered_map") m_containerType = MAP;
  }

public:
  ContainerType GetContainerType() const { return m_containerType; }
  const std::string& GetName() const { return m_name; }
  UINT32 GetId() const { return m_id; }
  bool IsBaseType() const { return m_parentClass == nullptr; }
  const TypeInfo *GetParentClass() const { return m_parentClass; }
  const TypesList& GetTemplateTypes() const { return m_templateTypes; }
  const EnumValuesList& GetEnumValues() const { return m_enumValues; }

  bool IsEnumType() const
  {
    return !m_enumValues.empty();
  }

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
  ContainerType m_containerType;
  std::string m_name;
  UINT32 m_id;
  const TypeInfo *m_parentClass;
  TypesList m_templateTypes;
  EnumValuesList m_enumValues;

};

template <typename Type>
class TypeInfoRegistry
{

public:
  static const TypeInfo s_typeInfo;

};
  
}
