/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector2Editor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector2 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfVector2Editor.hpp"
#include "Math/sfVector2.hpp"

namespace Sulfur
{

Vector2Editor::Vector2Editor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateEdits();
  UpdateValue();
}

Vector2Editor::Vector2Editor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateEdits();
  UpdateValue();
}

Vector2Editor::~Vector2Editor()
{
}

void Vector2Editor::UpdateValue()
{
  const Vector2& val = GetValue<Vector2>();
  m_edits[0]->setText(QString::number(val.GetX()));
  m_edits[1]->setText(QString::number(val.GetY()));
}

void Vector2Editor::CreateEdits()
{
  for (UINT32 i = 0; i < 2; ++i)
  {
    m_edits[i] = new QLineEdit();
    m_edits[i]->setValidator(new QDoubleValidator());
    m_layout->addWidget(m_edits[i]);

    QObject::connect(
      m_edits[i], &QLineEdit::textEdited,
      this, &Vector2Editor::OnValueChanged
      );
  }
}

void Vector2Editor::OnValueChanged(const QString& value)
{
  SetValue(Vector2(
    (Real)m_edits[0]->text().toDouble(),
    (Real)m_edits[1]->text().toDouble()
    ));
}

}
