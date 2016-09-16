/******************************************************************************/
/*!
\par     Sulfur
\file    sfStringEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   String property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfStringEditor.hpp"

namespace Sulfur
{

StringEditor::StringEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : PropertyEditor(object, prop, parent)
{
  m_edit = new QLineEdit();
  m_layout->addWidget(m_edit);

  QObject::connect(
    m_edit, &QLineEdit::textEdited,
    this, &StringEditor::OnValueChanged
    );

  UpdateValue();
}

StringEditor::~StringEditor()
{
}

void StringEditor::UpdateValue()
{
  m_edit->setText(GetValue<std::string>().c_str());
}


void StringEditor::OnValueChanged(const QString& value)
{
  SetValue(std::string(value.toUtf8().data()));
  UpdateValue();
}

}
