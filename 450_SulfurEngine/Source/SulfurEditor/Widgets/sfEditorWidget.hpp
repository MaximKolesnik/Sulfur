/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Scene browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfGameWidget.hpp"

namespace Sulfur
{
  class Object;	

  class EditorWidget : public QWidget
  {
    Q_OBJECT

  public:
    EditorWidget(QWidget *parent = 0);
    ~EditorWidget();

    GameWidget* GetGameWidget();

  private:
    void Setup();
    void UpdatePlayStateButtons();

  protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

  public slots:
    void OnTranslationToggled();
    void OnRotationToggled();
    void OnScalingToggled();

    void OnPlayToggled();
    void OnPauseToggled();
    void OnStopToggled();

  signals:
    void DeleteSelection();
    void DuplicateSelection();

  private:
    QGridLayout *m_layout;

    QPushButton *m_translationButton;
    QPushButton *m_rotationButton;
    QPushButton *m_scalingButton;

    QPushButton *m_playButton;
    QPushButton *m_pauseButton;
    QPushButton *m_stopButton;

    GameWidget *m_gameWidget;
  
  };
  
}