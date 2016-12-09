/******************************************************************************/
/*!
\par     Sulfur
\file    sfCollapsableEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/19/2016

\brief   Collapsable container property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfCollapsableEditor.hpp"

namespace Sulfur
{

CollapsableEditor::CollapsableEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : ContainerEditor(object, prop, parent)
{
  Setup();
}

CollapsableEditor::CollapsableEditor(void *ptr, QWidget *parent)
  : ContainerEditor(ptr, parent)
{
  Setup();
}

CollapsableEditor::~CollapsableEditor()
{
}

void CollapsableEditor::SetHeaderText(const std::string& text)
{
  m_collapseButton->setText(text.c_str());
}

void CollapsableEditor::Setup()
{
  m_headerLayout = new QHBoxLayout();
  m_headerLayout->setContentsMargins(0, 0, 0, 0);

  m_headerFrame = new QFrame();
  m_headerFrame->setContentsMargins(5, 5, 5, 5);

  m_headerFrame->setLayout(m_headerLayout);
  m_mainLayout->insertWidget(0, m_headerFrame);

  m_collapseButton = new QPushButton();
  m_collapseButton->setContentsMargins(0, 0, 0, 0);
  if (m_property != nullptr) m_collapseButton->setText(m_property->GetName().c_str());
  m_headerLayout->insertWidget(0, m_collapseButton);

  // Bold font
  QFont font = m_collapseButton->font();
  font.setBold(true);
  m_collapseButton->setFont(font);

  QObject::connect(
    m_collapseButton, &QPushButton::clicked,
    this, &CollapsableEditor::OnCollapseClicked
    );
}

void CollapsableEditor::OnCollapseClicked(bool checked)
{
  m_childrenWidget->setVisible(!m_childrenWidget->isVisible());
}

}
