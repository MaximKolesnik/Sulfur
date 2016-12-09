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
  : QScrollArea(parent), m_object(nullptr)
{
  CreateLayout();
  setMinimumSize(400, 175);
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);
  setWidgetResizable(true);
  //setStyleSheet("background-color: transparent;");
}

InspectorWidget::~InspectorWidget()
{
}

void InspectorWidget::UpdateValues()
{

}

void InspectorWidget::CreateLayout()
{
  m_frame = new QFrame();
  m_frame->setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Expanding);
  m_frame->setFixedWidth(size().width());
  m_layout = new QVBoxLayout();
  m_frame->setLayout(m_layout);
  setWidget(m_frame);
}

void InspectorWidget::OnPropertyChanged()
{
  emit ObjectChanged();
}

void InspectorWidget::resizeEvent(QResizeEvent *e)
{
  m_frame->setFixedWidth(e->size().width());
  QScrollArea::resizeEvent(e);
}

}
