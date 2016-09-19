/******************************************************************************/
/*!
\par     Sulfur
\file    sfEnumEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/19/2016

\brief   Enum property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfEnumEditor.hpp"

namespace Sulfur
{

EnumEditor::EnumEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateComboBox();
  UpdateValue();
}

EnumEditor::EnumEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateComboBox();
  UpdateValue();
}

EnumEditor::~EnumEditor()
{
}

void EnumEditor::UpdateValue()
{
  m_comboBox->setCurrentIndex(GetValue<UINT32>());
}

void EnumEditor::CreateComboBox()
{
  m_comboBox = new QComboBox();
  m_layout->addWidget(m_comboBox);

  QObject::connect(
    m_comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
    this, &EnumEditor::OnValueSelected
    );

  if (m_property != nullptr)
  {
    auto& enumValues = m_property->GetTypeInfo()->GetEnumValues();
    for (const std::string& value : enumValues)
      m_comboBox->addItem(value.c_str());
  }
}

void EnumEditor::OnValueSelected(int index)
{
  SetValue((UINT32)index);
  UpdateValue();
}

}
