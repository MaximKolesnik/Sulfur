/******************************************************************************/
/*!
\par     Sulfur
\file    sfInspectorWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Object inspector widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfInspectorWidget.hpp"
#include "Editors/sfPropertyEditor.hpp"
#include "Editors/sfReflectedObjectEditor.hpp"

namespace Sulfur
{

InspectorWidget::InspectorWidget(QWidget *parent)
  : QWidget(parent), m_object(nullptr)
{
  CreateLayout();
  setMinimumSize(300, 175);
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::UpdateValues()
{

}

void InspectorWidget::CreateLayout()
{
  m_layout = new QVBoxLayout();
  setLayout(m_layout);
}

void InspectorWidget::OnPropertyChanged()
{
  emit ObjectChanged();
}

}
