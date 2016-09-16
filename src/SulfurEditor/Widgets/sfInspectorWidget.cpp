/******************************************************************************/
/*!
\par     Sulfur
\file    sfInspectorWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Object inspector widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfInspectorWidget.hpp"

// Editors
#include "Editors/sfVector3Editor.hpp"
#include "Editors/sfQuaternionEditor.hpp"
#include "Editors/sfStringEditor.hpp"

namespace Sulfur
{

InspectorWidget::InspectorWidget(QWidget *parent)
  : QWidget(parent), m_object(nullptr)
{
  m_layout = new QVBoxLayout();
  setLayout(m_layout);
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::SetObject(ReflectionBase *object)
{
  m_object = object;

  ReflectionBase::PropertyList properties;
  object->GetProperties(properties);

  for (Property *prop : properties)
  {
    PropertyEditor *editor = nullptr;

    if (prop->GetTypeInfo()->IsDerivedFrom<ReflectionBase>()) editor = new PropertyEditor(m_object, prop);
    else if (prop->IsType<Vector3>()) editor = new Vector3Editor(m_object, prop);
    else if (prop->IsType<Quaternion>()) editor = new QuaternionEditor(m_object, prop);
    else if (prop->IsType<std::string>()) editor = new StringEditor(m_object, prop);
    else editor = new PropertyEditor(m_object, prop);

    m_layout->addWidget(editor, 0, Qt::AlignTop);
  }

  m_layout->insertStretch(-1, 1);
}

void InspectorWidget::UpdateValues()
{

}

}
