/******************************************************************************/
/*!
\par     Sulfur
\file    sfGameWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Game rendering widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Window/sfWindow.hpp"

namespace Sulfur
{
	
  class GameWidget : public QWidget
  {

    Q_OBJECT
    Q_DISABLE_COPY(GameWidget)

  public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void Frame();

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual QPaintEngine* paintEngine() const;

  protected:
    virtual void resizeEvent(QResizeEvent* evt) override;
    virtual void paintEvent(QPaintEvent* evt) override;
    virtual void timerEvent(QTimerEvent* evt) override;

  private:
    void UpdateEditorCamera();

  private:
    Window *m_window;

    bool m_controllingCamera;
    HNDL m_editorCamera;
    Real m_cameraYaw;
    Real m_cameraPitch;

    int m_resizeTimer;
  
  };
  
}