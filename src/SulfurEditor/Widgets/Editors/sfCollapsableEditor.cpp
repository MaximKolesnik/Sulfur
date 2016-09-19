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

void CollapsableEditor::Setup()
{
  m_collapseButton = new QPushButton();
  if (m_property != nullptr) m_collapseButton->setText(m_property->GetName().c_str());
  m_mainLayout->insertWidget(0, m_collapseButton);

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
