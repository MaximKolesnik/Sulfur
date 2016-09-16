/******************************************************************************/
/*!
\par     Sulfur
\file    sfQuaternionEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Quaternion property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfQuaternionEditor.hpp"

namespace Sulfur
{

QuaternionEditor::QuaternionEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : PropertyEditor(object, prop, parent)
{
  for (UINT32 i = 0; i < 3; ++i)
  {
    m_edits[i] = new QLineEdit();
    m_edits[i]->setValidator(new QDoubleValidator());
    m_layout->addWidget(m_edits[i]);

    QObject::connect(
      m_edits[i], &QLineEdit::textEdited,
      this, &QuaternionEditor::OnValueChanged
      );
  }  

  UpdateValue();
}

QuaternionEditor::~QuaternionEditor()
{
}

void QuaternionEditor::UpdateValue()
{
  const Quaternion& val = GetValue<Quaternion>();

  Real euler[3];
  val.GetEulerXYZ(euler[0], euler[1], euler[2]);

  m_edits[0]->setText(QString::number(euler[0] * SF_DEGS_PER_RAD));
  m_edits[1]->setText(QString::number(euler[1] * SF_DEGS_PER_RAD));
  m_edits[2]->setText(QString::number(euler[2] * SF_DEGS_PER_RAD));
}


void QuaternionEditor::OnValueChanged(const QString& value)
{
  Quaternion quat;
  quat.SetEuler(
    (Real)m_edits[0]->text().toDouble() * SF_RADS_PER_DEG,
    (Real)m_edits[1]->text().toDouble() * SF_RADS_PER_DEG,
    (Real)m_edits[2]->text().toDouble() * SF_RADS_PER_DEG
    );

  SetValue(quat);
}

}
