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

namespace Sulfur
{

InspectorWidget::InspectorWidget(QWidget *parent)
  : QWidget(parent), m_object(nullptr)
{
  m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
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
    QLabel *label = new QLabel();
    label->setText(prop->GetName().c_str());
    m_layout->addWidget(label);
  }
}

void InspectorWidget::UpdateValues()
{

}

}
