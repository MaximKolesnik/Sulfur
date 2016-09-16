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
#include "Math/sfVector2.hpp"
#include "Math/sfVector3.hpp"
#include "Math/sfVector4.hpp"
#include "Math/sfQuaternion.hpp"
#include "Math/sfMatrix3.hpp"
#include "Math/sfMatrix4.hpp"

#define SF_REGISTER_TYPE(type, base) \
template <> const TypeInfo Sulfur::TypeInfoRegistry<type>::s_typeInfo = TypeInfo(#type, TypeInfo::IdGen(), base); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::vector<type>>::s_typeInfo = TypeInfo("std::vector<type>", TypeInfo::IdGen(), base); \
template <> const TypeInfo Sulfur::TypeInfoRegistry<std::list<type>>::s_typeInfo = TypeInfo("std::list<type>", TypeInfo::IdGen(), base);

#define SF_REGISTER_BASE_TYPE(type) SF_REGISTER_TYPE(type, true)
#define SF_REGISTER_REFLECTED_TYPE(type) SF_REGISTER_TYPE(type, false)
#define SF_TYPE_INFO(type) (&Sulfur::TypeInfoRegistry<type>::s_typeInfo)
#define SF_TYPE_ID(type) (SF_TYPE_INFO(type)->GetId())

namespace Sulfur
{
	
  class TypeInfo
  {
  
  public:
    TypeInfo(const std::string& name, UINT32 id, bool baseType, const TypeInfo *parentClass = nullptr)
      : m_name(name), m_id(id), m_baseType(baseType), m_parentClass(parentClass)
    {}

    const std::string& GetName() const { return m_name; }
    UINT32 GetId() const { return m_id; }
    bool IsBaseType() const { return m_baseType; }
    const TypeInfo *GetParentClass() const { return m_parentClass; }

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
    bool m_baseType;
    const TypeInfo *m_parentClass;

  };

  template <typename Type>
  class TypeInfoRegistry
  {

  public:
    static const TypeInfo s_typeInfo;

  };

  SF_REGISTER_BASE_TYPE(UINT16)
  SF_REGISTER_BASE_TYPE(UINT32)
  SF_REGISTER_BASE_TYPE(UINT64)
  SF_REGISTER_BASE_TYPE(Real)
  SF_REGISTER_BASE_TYPE(std::string)
  SF_REGISTER_BASE_TYPE(Vector2)
  SF_REGISTER_BASE_TYPE(Vector3)
  SF_REGISTER_BASE_TYPE(Vector4)
  SF_REGISTER_BASE_TYPE(Quaternion)
  SF_REGISTER_BASE_TYPE(Matrix3)
  SF_REGISTER_BASE_TYPE(Matrix4)
  
}