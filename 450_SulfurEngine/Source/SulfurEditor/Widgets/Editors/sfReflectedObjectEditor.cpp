/******************************************************************************/
/*!
\par     Sulfur
\file    sfReflectedObjectEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Editor for reflected objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfReflectedObjectEditor.hpp"

namespace Sulfur
{

ReflectedObjectEditor::ReflectedObjectEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : CollapsableEditor(object, prop, parent)
{
  UpdateValue();
}

ReflectedObjectEditor::ReflectedObjectEditor(void *ptr, QWidget *parent)
  : CollapsableEditor(ptr, parent)
{
  UpdateValue();
}

ReflectedObjectEditor::~ReflectedObjectEditor()
{
}

void ReflectedObjectEditor::UpdateValue()
{
  //ClearLayout();

  m_object = const_cast<ReflectionBase*>(&GetValue<ReflectionBase>());
  ReflectionBase::PropertyList properties;
  m_object->GetProperties(properties);

  m_collapseButton->setText(m_object->GetTypeInfo()->GetName().c_str());
  for (Property *prop : properties)
  {
    if (prop->GetName() != "Name") // TODO: Find a better way to do this
    {
      AddChild(PropertyEditor::Create(m_object, prop));
    }
  }
}

}
