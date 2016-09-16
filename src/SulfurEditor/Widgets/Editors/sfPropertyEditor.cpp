/******************************************************************************/
/*!
\par     Sulfur
\file    sfPropertyEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Base property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfPropertyEditor.hpp"

namespace Sulfur
{

PropertyEditor::PropertyEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : QWidget(parent), m_object(object), m_property(prop)
{
  m_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  m_layout->setMargin(0);
  setLayout(m_layout);
  setContentsMargins(0, 0, 0, 0);

  QLabel *label = new QLabel();
  label->setText(prop->GetName().c_str());
  label->setMinimumWidth(125);
  m_layout->addWidget(label);
}

PropertyEditor::~PropertyEditor()
{
}

}
