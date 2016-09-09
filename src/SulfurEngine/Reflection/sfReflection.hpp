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

namespace Sulfur
{
	
  template <typename Type, std::string(*getProperty)()>
  struct StaticPropertyRegister
  {
    static void* Register()
    {
      Type::RegisterProperty((*getProperty)());
      return nullptr;
    }

    static const void* DUMMY;

  };

  template <typename Type, std::string(*getProperty)()>
  const void* StaticPropertyRegister<Type, getProperty>::DUMMY = Register();

  class ReflectionBase
  {

  public:
    typedef std::vector<std::string> PropertyList;
    typedef std::unordered_map<std::string, std::string> PropertyMap;
    template <typename Type, std::string(*getProperty)()> friend struct StaticPropertyRegister;
  
  public:
    virtual std::string GetProperty(const std::string& name) const
    {
      PropertyMap& propertyMap = GetPropertyMap();
      auto it = propertyMap.find(name);
      
      SF_CRITICAL_ERR_EXP(
        it == propertyMap.end(),
        "No property with the name given."
        );

      return it->second;
    }

    virtual void GetProperties(PropertyList& list) const
    {
      list.insert(list.end(), GetPropertyList().begin(), GetPropertyList().end());
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
    static const std::string Property(const std::string& name)
    {
      auto it = GetPropertyMap().find(name);
      if (it == GetPropertyMap().end())
        return "";

      return it->second;
    }

  };
  
}

#define SF_REFLECTED_CLASS_DERIVED(name, baseType) \
class name : public baseType { \
private: \
typedef name ThisType; \
protected: \
template <typename Type, std::string(*getProperty)()> friend struct StaticPropertyRegister; \
static PropertyList& GetPropertyList() { static PropertyList propertyList; return propertyList; } \
static PropertyMap& GetPropertyMap() { static PropertyMap propertyMap; return propertyMap; } \
static void RegisterProperty(const std::string& prop) { GetPropertyList().push_back(prop); GetPropertyMap()[prop] = prop; } \
public: \
virtual std::string GetProperty(const std::string& name) const { \
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
static const std::string Property(const std::string& name) { \
  auto it = GetPropertyMap().find(name); \
  if (it == GetPropertyMap().end()) \
    return baseType::Property(name); \
  return GetPropertyMap()[name]; \
} \
private:

#define SF_REFLECTED_CLASS(name) SF_REFLECTED_CLASS_DERIVED(name, ReflectionBase)

#define SF_PRIVATE_PROPERTY(type, name, upperName, display) \
private: \
static std::string upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return display; } \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; } \
  void Set##upperName(const type& value) { m_##name = value; } \

#define SF_PROTECTED_PROPERTY(type, name, upperName, display) \
private: \
static std::string upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return display; } \
protected: \
type m_##name; \
public: \
  const type& Get##upperName() const { return m_##name; } \
  void Set##upperName(const type& value) { m_##name = value; } \

#define SF_PUBLIC_PROPERTY(type, name, upperName, display) \
private: \
static std::string upperName##Property() { (void)StaticPropertyRegister<ThisType, &upperName##Property>::DUMMY; return display; } \
public: \
type m_##name;
