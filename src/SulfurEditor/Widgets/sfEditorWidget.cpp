/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Scene browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfEditorWidget.hpp"

namespace Sulfur
{

EditorWidget::EditorWidget(QWidget *parent)
  : QWidget(parent)
{
  Setup();
}

EditorWidget::~EditorWidget()
{
}

void EditorWidget::Setup()
{
  setContentsMargins(0, 0, 0, 0);
  setMinimumSize(350, 350);
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);

  m_layout = new QGridLayout();
  m_layout->setMargin(0);
  setLayout(m_layout);

  m_translationButton = new QPushButton("T");
  m_translationButton->setCheckable(true);
  m_translationButton->setChecked(true);
  m_translationButton->setFixedWidth(100);
  m_layout->addWidget(m_translationButton, 0, 0);

  QObject::connect(
    m_translationButton, &QPushButton::clicked,
    this, &EditorWidget::OnTranslationToggled
    );

  m_rotationButton = new QPushButton("R");
  m_rotationButton->setCheckable(true);
  m_rotationButton->setFixedWidth(100);
  m_layout->addWidget(m_rotationButton, 0, 1);

  QObject::connect(
    m_rotationButton, &QPushButton::clicked,
    this, &EditorWidget::OnRotationToggled
    );

  m_scalingButton = new QPushButton("S");
  m_scalingButton->setCheckable(true);
  m_scalingButton->setFixedWidth(100);
  m_layout->addWidget(m_scalingButton, 0, 2);

  QObject::connect(
    m_scalingButton, &QPushButton::clicked,
    this, &EditorWidget::OnScalingToggled
    );

  m_gameWidget = new GameWidget();
  m_layout->addWidget(m_gameWidget, 1, 0, 1, 4);
}

GameWidget* EditorWidget::GetGameWidget()
{
  return m_gameWidget;
}

void EditorWidget::OnTranslationToggled()
{
  m_translationButton->setChecked(true);
  m_rotationButton->setChecked(false);
  m_scalingButton->setChecked(false);
  m_gameWidget->SetGizmo(GameWidget::TRANSLATION_GIZMO);
}

void EditorWidget::OnRotationToggled()
{
  m_translationButton->setChecked(false);
  m_rotationButton->setChecked(true);
  m_scalingButton->setChecked(false);
  m_gameWidget->SetGizmo(GameWidget::ROTATION_GIZMO);
}

void EditorWidget::OnScalingToggled()
{
  m_translationButton->setChecked(false);
  m_rotationButton->setChecked(false);
  m_scalingButton->setChecked(true);
  m_gameWidget->SetGizmo(GameWidget::SCALING_GIZMO);
}


}
