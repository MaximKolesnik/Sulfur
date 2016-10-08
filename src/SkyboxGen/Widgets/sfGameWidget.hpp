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

  private:
    void UpdateEditorCamera();

  private:
    bool m_controllingCamera;
    HNDL m_editorCamera;
  
  };
  
}