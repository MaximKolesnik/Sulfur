/******************************************************************************/
/*!
\par     Sulfur
\file    sfBoolEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Boolean property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfBoolEditor.hpp"

namespace Sulfur
{

  BoolEditor::BoolEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateCheckBox();
  UpdateValue();
}

BoolEditor::BoolEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateCheckBox();
  UpdateValue();
}

BoolEditor::~BoolEditor()
{
}

void BoolEditor::UpdateValue()
{
  m_checkBox->setCheckState(GetValue<bool>() ? Qt::Checked : Qt::Unchecked);
}

void BoolEditor::CreateCheckBox()
{
  m_checkBox = new QCheckBox();
  m_checkBox->setCheckable(true);
  m_layout->addWidget(m_checkBox);

  QObject::connect(
    m_checkBox, &QCheckBox::stateChanged,
    this, &BoolEditor::OnStateChanged
    );
}

void BoolEditor::OnStateChanged(int state)
{
  SetValue(state == Qt::Checked ? true : false);
}

}
