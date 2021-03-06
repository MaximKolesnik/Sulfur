/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWindow.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Main editor window

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <QtWidgets/QMainWindow>
#include "Widgets/sfGameWidget.hpp"

namespace Sulfur
{

  class EditorWindow : public QMainWindow
  {

    Q_OBJECT

  public:
    EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    void Frame();

  private:
    void CreateMenuBar();

  private:
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;

    GameWidget *m_game;

  };

}
