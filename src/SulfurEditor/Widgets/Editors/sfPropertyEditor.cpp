/******************************************************************************/
/*!
\par     Sulfur
\file    sfPropertyEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Base property editor

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfPropertyEditor.hpp"
#include "Types/sfObject.hpp"

// Editors
#include "sfObjectEditor.hpp"
#include "sfReflectedObjectEditor.hpp"
#include "sfBoolEditor.hpp"
#include "sfRealEditor.hpp"
#include "sfVector2Editor.hpp"
#include "sfVector3Editor.hpp"
#include "sfVector4Editor.hpp"
#include "sfColorEditor.hpp"
#include "sfQuaternionEditor.hpp"
#include "sfStringEditor.hpp"
#include "sfEnumEditor.hpp"
#include "sfResourceEditor.hpp"

namespace Sulfur
{

PropertyEditor::PropertyEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : QFrame(parent), m_object(object), m_property(prop), m_ptr(nullptr)
{
}

PropertyEditor::PropertyEditor(void *ptr, QWidget *parent)
  : QFrame(parent), m_object(nullptr), m_property(nullptr), m_ptr(ptr)
{
}

PropertyEditor::~PropertyEditor()
{
}

template <typename...Args>
PropertyEditor* CreatePropertyEditor(const TypeInfo *typeInfo, Args...args)
{
  switch (typeInfo->GetContainerType())
  {
  case VECTOR: return new PropertyEditor(args...);
  case LIST: return new PropertyEditor(args...);
  case MAP: return new PropertyEditor(args...);
  case NONE:
    if (typeInfo->IsEnumType()) return new EnumEditor(args...);
    if (typeInfo == SF_TYPE_INFO(Object)) return new ObjectEditor(args...);
    if (typeInfo->IsDerivedFrom<ReflectionBase>()) return new ReflectedObjectEditor(args...);
    if (typeInfo == SF_TYPE_INFO(bool)) return new BoolEditor(args...);
    if (typeInfo == SF_TYPE_INFO(Real)) return new RealEditor(args...);
    if (typeInfo == SF_TYPE_INFO(Vector2)) return new Vector2Editor(args...);
    if (typeInfo == SF_TYPE_INFO(Vector3)) return new Vector3Editor(args...);
    if (typeInfo == SF_TYPE_INFO(Vector4)) return new Vector4Editor(args...);
    if (typeInfo == SF_TYPE_INFO(Color)) return new ColorEditor(args...);
    if (typeInfo == SF_TYPE_INFO(Quaternion)) return new QuaternionEditor(args...);
    if (typeInfo == SF_TYPE_INFO(std::string)) return new StringEditor(args...);
    if (typeInfo == SF_TYPE_INFO(ResourcePath)) return new ResourceEditor(args...);
  }

  return nullptr;
  //return new PropertyEditor(args...);
}

PropertyEditor* PropertyEditor::Create(ReflectionBase *object, Property *prop)
{
  return CreatePropertyEditor(prop->GetTypeInfo(), object, prop);
}

PropertyEditor* PropertyEditor::PropertyEditor::Create(void *ptr, const TypeInfo *typeInfo)
{
  return CreatePropertyEditor(typeInfo, ptr);
}

}
