/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector4Editor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector4 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfVector4Editor.hpp"
#include "Math/sfVector4.hpp"

namespace Sulfur
{

Vector4Editor::Vector4Editor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateEdits();
  UpdateValue();
}

Vector4Editor::Vector4Editor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateEdits();
  UpdateValue();
}

Vector4Editor::~Vector4Editor()
{
}

void Vector4Editor::UpdateValue()
{
  const Vector4& val = GetValue<Vector4>();
  m_edits[0]->setText(QString::number(val.GetX()));
  m_edits[1]->setText(QString::number(val.GetY()));
  m_edits[2]->setText(QString::number(val.GetZ()));
  m_edits[3]->setText(QString::number(val.GetW()));
}

void Vector4Editor::CreateEdits()
{
  for (UINT32 i = 0; i < 4; ++i)
  {
    m_edits[i] = new QLineEdit();
    m_edits[i]->setValidator(new QDoubleValidator());
    m_layout->addWidget(m_edits[i]);

    QObject::connect(
      m_edits[i], &QLineEdit::textEdited,
      this, &Vector4Editor::OnValueChanged
      );
  }
}

void Vector4Editor::OnValueChanged(const QString& value)
{
  SetValue(Vector4(
    (Real)m_edits[0]->text().toDouble(),
    (Real)m_edits[1]->text().toDouble(),
    (Real)m_edits[2]->text().toDouble(),
    (Real)m_edits[3]->text().toDouble()
    ));
}

}
