/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeletableEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   Deleteable container property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfDeletableEditor.hpp"

namespace Sulfur
{

DeletableEditor::DeletableEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : ReflectedObjectEditor(object, prop, parent)
{
  Setup();
}

DeletableEditor::DeletableEditor(void *ptr, QWidget *parent)
  : ReflectedObjectEditor(ptr, parent)
{
  Setup();
}

DeletableEditor::~DeletableEditor()
{
}

void DeletableEditor::Setup()
{
  m_deleteButton = new QPushButton("x");
  m_deleteButton->setFixedWidth(32);
  m_headerLayout->insertWidget(1, m_deleteButton);

  QObject::connect(
    m_deleteButton, &QPushButton::clicked,
    this, &DeletableEditor::OnDeleteClicked
  );
}

void DeletableEditor::OnDeleteClicked(bool checked)
{
  emit Deleted();
  delete this;
}

}
