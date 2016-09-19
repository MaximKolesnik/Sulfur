/******************************************************************************/
/*!
\par     Sulfur
\file    sfRealEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Real property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRealEditor.hpp"

namespace Sulfur
{

RealEditor::RealEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateEdit();
  UpdateValue();
}

RealEditor::RealEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateEdit();
  UpdateValue();
}

RealEditor::~RealEditor()
{
}

void RealEditor::UpdateValue()
{
  m_edit->setText(QString::number(GetValue<Real>()));
}

void RealEditor::CreateEdit()
{
  m_edit = new QLineEdit();
  m_edit->setValidator(new QDoubleValidator());
  m_layout->addWidget(m_edit);

  QObject::connect(
    m_edit, &QLineEdit::textEdited,
    this, &RealEditor::OnValueChanged
    );
}

void RealEditor::OnValueChanged(const QString& value)
{
  SetValue((Real)value.toDouble());
  UpdateValue();
}

}
