/******************************************************************************/
/*!
\par     Sulfur
\file    sfLabeledEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Property editor with horizontal layout and text label

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfLabeledEditor.hpp"

namespace Sulfur
{

LabeledEditor::LabeledEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : PropertyEditor(object, prop, parent)
{
  CreateLayout();
  CreateLabel();
}

LabeledEditor::LabeledEditor(void *ptr, QWidget *parent)
  : PropertyEditor(ptr, parent)
{
  CreateLayout();
}

LabeledEditor::~LabeledEditor()
{
}

void LabeledEditor::CreateLayout()
{
  m_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
  m_layout->setMargin(0);
  setLayout(m_layout);
  setContentsMargins(0, 0, 0, 0);
}

void LabeledEditor::CreateLabel()
{
  m_label = new QLabel();
  m_label->setText(m_property->GetName().c_str());
  m_label->setMinimumWidth(125);
  m_label->setMaximumWidth(125);
  m_layout->addWidget(m_label);

  // Bold font
  QFont font = m_label->font();
  font.setBold(true);
  m_label->setFont(font);
}

}
