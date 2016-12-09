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

  m_translationButton = new QPushButton();
  m_translationButton->setCheckable(true);
  m_translationButton->setChecked(true);
  m_translationButton->setIcon(QIcon("images/TranslationTool.png"));
  m_translationButton->setIconSize(QSize(32, 32));
  m_translationButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_translationButton, 0, 0);

  QObject::connect(
    m_translationButton, &QPushButton::clicked,
    this, &EditorWidget::OnTranslationToggled
  );

  m_rotationButton = new QPushButton();
  m_rotationButton->setCheckable(true);
  m_rotationButton->setIcon(QIcon("images/RotationTool.png"));
  m_rotationButton->setIconSize(QSize(32, 32));
  m_rotationButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_rotationButton, 0, 1);

  QObject::connect(
    m_rotationButton, &QPushButton::clicked,
    this, &EditorWidget::OnRotationToggled
  );

  m_scalingButton = new QPushButton();
  m_scalingButton->setCheckable(true);
  m_scalingButton->setIcon(QIcon("images/ScaleTool.png"));
  m_scalingButton->setIconSize(QSize(32, 32));
  m_scalingButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_scalingButton, 0, 2);

  QObject::connect(
    m_scalingButton, &QPushButton::clicked,
    this, &EditorWidget::OnScalingToggled
  );

  m_playButton = new QPushButton();
  m_playButton->setCheckable(true);
  m_playButton->setIcon(QIcon("images/Play.png"));
  m_playButton->setIconSize(QSize(32, 32));
  m_playButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_playButton, 0, 4);

  QObject::connect(
    m_playButton, &QPushButton::clicked,
    this, &EditorWidget::OnPlayToggled
  );

  m_pauseButton = new QPushButton();
  m_pauseButton->setCheckable(true);
  m_pauseButton->setIcon(QIcon("images/Pause.png"));
  m_pauseButton->setIconSize(QSize(32, 32));
  m_pauseButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_pauseButton, 0, 5);

  QObject::connect(
    m_pauseButton, &QPushButton::clicked,
    this, &EditorWidget::OnPauseToggled
  );

  m_stopButton = new QPushButton();
  m_stopButton->setCheckable(true);
  m_stopButton->setChecked(true);
  m_stopButton->setIcon(QIcon("images/Stop.png"));
  m_stopButton->setIconSize(QSize(32, 32));
  m_stopButton->setFixedSize(QSize(36, 36));
  m_layout->addWidget(m_stopButton, 0, 6);

  QObject::connect(
    m_stopButton, &QPushButton::clicked,
    this, &EditorWidget::OnStopToggled
  );

  m_gameWidget = new GameWidget();
  m_layout->addWidget(m_gameWidget, 1, 0, 1, 7);
}

void EditorWidget::UpdatePlayStateButtons()
{
  GameWidget::State state = m_gameWidget->GetState();
  m_playButton->setChecked(state == GameWidget::PLAYING);
  m_pauseButton->setChecked(state == GameWidget::PAUSED);
  m_stopButton->setChecked(state == GameWidget::STOPPED);
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

void EditorWidget::OnPlayToggled()
{
  m_gameWidget->SetState(GameWidget::PLAYING);
  UpdatePlayStateButtons();
}

void EditorWidget::OnPauseToggled()
{
  m_gameWidget->SetState(GameWidget::PAUSED);
  UpdatePlayStateButtons();
}

void EditorWidget::OnStopToggled()
{
  m_gameWidget->SetState(GameWidget::STOPPED);
  UpdatePlayStateButtons();
}


}
