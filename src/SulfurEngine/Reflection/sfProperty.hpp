/******************************************************************************/
/*!
\par     Sulfur
\file    sfProperty.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/9/2016

\brief   Reflection Properties

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfSerialization.hpp"
#include "sfTypeInfo.hpp"

namespace Sulfur
{

  class Property
  {
  
  public:
    Property(const std::string& name)
      : m_name(name)
    {
    }

    const std::string GetName()
    {
      return m_name;
    }

    template <typename PropertyType, typename ClassType>
    const PropertyType& GetValue(const ClassType& classInstance)
    {
      const void *val = GetValue(reinterpret_cast<const void *>(&classInstance));
      return *reinterpret_cast<const PropertyType*>(val);
    }

    template <typename PropertyType, typename ClassType>
    void SetValue(ClassType& classInstance, const PropertyType& value)
    {
      SetValue(reinterpret_cast<void *>(&classInstance), reinterpret_cast<const void *>(&value));
    }

    template <typename Type>
    bool IsType() const
    {
      return SF_TYPE_ID(Type) == GetTypeInfo()->GetId();
    }

    virtual const TypeInfo* GetTypeInfo() const = 0;

  public:
    virtual void Serialize(std::ostream& str, const void *classInstance) = 0;
    virtual void Deserialize(std::istream& str, void *classInstance) = 0;

  protected:
    virtual const void* GetValue(const void *classInstance) = 0;
    virtual void SetValue(void *classInstance, const void *value) = 0;

  private:
    std::string m_name;

  };

  template <typename ClassType, typename PropertyType>
  class TypedProperty : public Property
  {

  public:
    TypedProperty(const std::string& name) : Property(name) {}

    virtual void Serialize(std::ostream& str, const void *classInstance)
    {
      Serialization::Serialize(str, *reinterpret_cast<const PropertyType*>(GetValue(classInstance)));
    }

    virtual void Deserialize(std::istream& str, void *classInstance)
    {
      PropertyType value;
      Serialization::Deserialize(str, value);
      SetValue(classInstance, reinterpret_cast<const void *>(&value));
    }

    virtual const TypeInfo* GetTypeInfo() const override
    {
      return &TypeInfoRegistry<PropertyType>::s_typeInfo;
    }

  };

  template <typename ClassType, typename PropertyType>
  class GetterSetterProperty : public TypedProperty<ClassType, PropertyType>
  {

  public:
    typedef const PropertyType&(ClassType::*GetterFunction)() const;
    typedef void (ClassType::*SetterFunction)(const PropertyType&);

  public:
    GetterSetterProperty(const std::string& name, GetterFunction getter, SetterFunction setter)
      : TypedProperty(name), m_getter(getter), m_setter(setter)
    {
    }

  protected:
    virtual const void* GetValue(const void *classInstance)
    {
      SF_CRITICAL_ERR_EXP(
        m_getter != nullptr,
        "Property is write-only"
        );

      const ClassType& ci = *reinterpret_cast<const ClassType*>(classInstance);
      return reinterpret_cast<const void *>(&(ci.*m_getter)());
    }

    virtual void SetValue(void *classInstance, const void *value)
    {
      SF_CRITICAL_ERR_EXP(
        m_setter != nullptr,
        "Property is read-only"
        );
      if (m_setter == nullptr)
        return;

      ClassType& ci = *reinterpret_cast<ClassType*>(classInstance);
      const PropertyType& val = *reinterpret_cast<const PropertyType*>(value);
      (ci.*m_setter)(val);
    }

  private:
    GetterFunction m_getter;
    SetterFunction m_setter;

  };

  template <typename ClassType, typename PropertyType>
  class DirectAccessProperty : public TypedProperty<ClassType, PropertyType>
  {

  public:
    DirectAccessProperty(const std::string& name, UINT32 offset)
      : TypedProperty(name), m_offset(offset)
    {
    }

    virtual const void* GetValue(const void *classInstance) override
    {
      return reinterpret_cast<const void*>(reinterpret_cast<const BYTE*>(classInstance) + m_offset);
    }

    virtual void SetValue(void *classInstance, const void *value) override
    {
      PropertyType& memberRef = *reinterpret_cast<PropertyType*>(reinterpret_cast<BYTE*>(classInstance) + m_offset);
      memberRef = *reinterpret_cast<const PropertyType*>(value);
    }

  private:
    UINT32 m_offset;

  };
  
}