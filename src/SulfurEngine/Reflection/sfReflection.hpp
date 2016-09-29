/******************************************************************************/
/*!
\par     Sulfur
\file    sfReflection.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/7/2016

\brief   Reflection interface and macros

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

#include "sfProperty.hpp"
#include "Error\sfError.hpp"

namespace Sulfur
{
	
  template <typename Type, Property*(*getProperty)()>
  struct StaticPropertyRegister
  {
    static void* Register()
    {
      Type::RegisterProperty((*getProperty)());
      return nullptr;
    }

    static const void* DUMMY;

  };

  template <typename Type, Property*(*getProperty)()>
  const void* StaticPropertyRegister<Type, getProperty>::DUMMY = Register();

  class ReflectionBase
  {

  public:
    typedef std::vector<Property*> PropertyList;
    typedef std::unordered_map<std::string, Property*> PropertyMap;
    template <typename Type, Property*(*getProperty)()> friend struct StaticPropertyRegister;
  
  public:
    template <typename PropertyType>
    const PropertyType& GetPropertyValue(const std::string& name)
    {
      return GetProperty(name)->GetValue<PropertyType>(*this);
    }

    template <typename PropertyType>
    void SetPropertyValue(const std::string& name, const PropertyType& value)
    {
      return GetProperty(name)->SetValue(*this, value);
    }

    virtual Property* GetProperty(const std::string& name) const
    {
      PropertyMap& propertyMap = GetPropertyMap();
      auto it = propertyMap.find(name);
      
      if (it == propertyMap.end())
        return nullptr;

      return it->second;
    }

    virtual void GetProperties(PropertyList& list) const
    {
      list.insert(list.end(), GetPropertyList().begin(), GetPropertyList().end());
    }

    virtual const TypeInfo* GetTypeInfo() const
    {
      return SF_TYPE_INFO(ReflectionBase);
    }

  protected:
    static PropertyList& GetPropertyList() 
    { 
      static PropertyList propertyList;
      return propertyList; 
    }

    static PropertyMap& GetPropertyMap() 
    { 
      static PropertyMap propertyMap; 
      return propertyMap; 
    }

    static void Properties(PropertyList& list) { list.insert(list.end(), GetPropertyList().begin(), GetPropertyList().end()); }
    static Property* FindProperty(const std::string& name)
    {
      auto it = GetPropertyMap().find(name);
      if (it == GetPropertyMap().end())
        return nullptr;

      return it->second;
    }

  };

  template <> const TypeInfo TypeInfoRegistry<ReflectionBase>::s_typeInfo("ReflectionBase", TypeInfo::IdGen(), nullptr, {}, {});
  
}

#define SF_REFLECTED_CLASS_DERIVED(name, baseType) \
class name; \
SF_REGISTER_REFLECTED_TYPE(name, baseType) \
class name : public baseType { \
private: \
typedef name ThisType; \
protected: \
template <typename Type, Property*(*getProperty)()> friend struct StaticPropertyRegister; \
static PropertyList& GetPropertyList() { static PropertyList propertyList; return propertyList; } \
static PropertyMap& GetPropertyMap() { static PropertyMap propertyMap; return propertyMap; } \
static void RegisterProperty(Property *prop) { GetPropertyList().push_back(prop); GetPropertyMap()[prop->GetName()] = prop; } \
public: \
virtual const TypeInfo* GetTypeInfo() const override { return SF_TYPE_INFO(name); } \
virtual Property* GetProperty(const std::string& name) const { \
  PropertyMap& propertyMap = GetPropertyMap(); \
  auto it = propertyMap.find(name); \
  if (it == propertyMap.end()) \
    return baseType::GetProperty(name); \
  return it->second; \
} \
virtual void GetProperties(PropertyList& list) const { \
  baseType::GetProperties(list); \
  list.insert(list.end(), GetPropertyList().begin(), GetPropertyList().end()); \
} \
static void Properties(PropertyList& list) { baseType::Properties(list); list.insert(list.end(), GetPropertyList().begin(), GetPropertyList().end()); } \
static Property* FindProperty(const std::string& name) { \
  auto it = GetPropertyMap().find(name); \
  if (it == GetPropertyMap().end()) \
    return baseType::FindProperty(name); \
  return GetPropertyMap()[name]; \
} \
private:

#define SF_REFLECTED_CLASS(name) SF_REFLECTED_CLASS_DERIVED(name, ReflectionBase)

#define CONCAT(a, b) a##b

#define SF_RESOURCE(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, std::string>(display, &CONCAT(Get##upperName, ResourcePath), &Set##upperName); } \
type *CONCAT(m_##name, Resource); \
std::string CONCAT(m_##name, ResourcePath); \
public: \
  type* Get##upperName() const { return CONCAT(m_##name, Resource); } \
  const std::string& CONCAT(Get##upperName, ResourcePath) const { return CONCAT(m_##name, ResourcePath); } \
  void Set##upperName(const std::string& resourcePath) { m_##name = ResourceManager<type>::LoadResource(resourcePath); m_resource##upperName = resourcePath; }


#define SF_PRIVATE_PROPERTY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(display, &Get##upperName, &Set##upperName); } \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; } \
  void Set##upperName(const type& value) { m_##name = value; }

#define SF_PRIVATE_PROPERTY_READ_ONLY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(display, &Get##upperName, nullptr); } \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; }

#define SF_PRIVATE_PROPERTY_WRITE_ONLY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(display, nullptr, &Set##upperName); } \
type m_##name; \
public: \
  void Set##upperName(const type& value) { m_##name = value; }

#define SF_PROTECTED_PROPERTY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(display, &Get##upperName, &Set##upperName); } \
protected: \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; } \
  void Set##upperName(const type& value) { m_##name = value; }

#define SF_PROTECTED_PROPERTY_READ_ONLY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(#upperName, &Get##upperName, nullptr); } \
protected: \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; }

#define SF_PROTECTED_PROPERTY_WRITE_ONLY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new GetterSetterProperty<ThisType, type>(display, nullptr, &Set##upperName); } \
protected: \
type m_##name; \
public: \
  void Set##upperName(const type& value) { m_##name = value; }

#define SF_PUBLIC_PROPERTY(type, name, upperName, display) \
private: \
static Property* upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return new DirectAccessProperty<ThisType, type>(display, (UINT32)offsetof(ThisType, m_##name)); } \
public: \
type m_##name;
