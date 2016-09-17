/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector3Editor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector3 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfVector3Editor.hpp"
#include "Math/sfVector3.hpp"

namespace Sulfur
{

Vector3Editor::Vector3Editor(ReflectionBase *object, Property *prop, QWidget *parent)
  : PropertyEditor(object, prop, parent)
{
  for (UINT32 i = 0; i < 3; ++i)
  {
    m_edits[i] = new QLineEdit();
    m_edits[i]->setValidator(new QDoubleValidator());
    m_layout->addWidget(m_edits[i]);

    QObject::connect(
      m_edits[i], &QLineEdit::textEdited,
      this, &Vector3Editor::OnValueChanged
      );
  }  

  UpdateValue();
}

Vector3Editor::~Vector3Editor()
{
}

void Vector3Editor::UpdateValue()
{
  const Vector3& val = GetValue<Vector3>();
  m_edits[0]->setText(QString::number(val.GetX()));
  m_edits[1]->setText(QString::number(val.GetY()));
  m_edits[2]->setText(QString::number(val.GetZ()));
}


void Vector3Editor::OnValueChanged(const QString& value)
{
  SetValue(Vector3(
    (Real)m_edits[0]->text().toDouble(),
    (Real)m_edits[1]->text().toDouble(),
    (Real)m_edits[2]->text().toDouble()
    ));

  UpdateValue();
}

}
