/******************************************************************************/
/*!
\par     Sulfur
\file    sfContainerEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Base property editor for container types (vectors, lists, etc)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfContainerEditor.hpp"

namespace Sulfur
{

ContainerEditor::ContainerEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : PropertyEditor(object, prop, parent), m_childrenLayout(nullptr)
{
  Setup();
}

ContainerEditor::ContainerEditor(void *ptr, QWidget *parent)
  : PropertyEditor(ptr, parent), m_childrenLayout(nullptr)
{
  Setup();
}

ContainerEditor::~ContainerEditor()
{
}

void ContainerEditor::Setup()
{
  m_mainLayout = CreateLayout();
  setLayout(m_mainLayout);

  m_childrenLayout = CreateLayout();
  setContentsMargins(0, 0, 0, 0);

  //m_collapseButton = new QPushButton();
  //if (m_property != nullptr) m_collapseButton->setText(m_property->GetName().c_str());
  //m_mainLayout->addWidget(m_collapseButton);

  m_childrenWidget = new QWidget();
  m_childrenWidget->setLayout(m_childrenLayout);
  m_childrenWidget->setContentsMargins(0, 0, 0, 0);
  m_mainLayout->addWidget(m_childrenWidget);
}

void ContainerEditor::ClearLayout()
{
  QBoxLayout *newLayout = CreateLayout();
  m_childrenWidget->setLayout(newLayout);

  delete m_childrenLayout;
  m_childrenLayout = newLayout;
}

QBoxLayout* ContainerEditor::CreateLayout()
{
  QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
  layout->setMargin(0);
  return layout;
}

}
