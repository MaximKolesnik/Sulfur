/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWindow.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Main editor window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfEditorWindow.hpp"
#include "Widgets/sfGameWidget.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Factories/sfObjectFactory.hpp"

namespace Sulfur
{

EditorWindow::EditorWindow(QWidget *parent)
  : QMainWindow(parent)
{
  resize(1920, 1080);

  m_game = new GameWidget();
  setCentralWidget(m_game);

  setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea);

  CreateMenuBar();

  m_toolBar = new QToolBar();
  addToolBar(m_toolBar);
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::Frame()
{
  m_game->Frame();
}

void EditorWindow::CreateMenuBar()
{
  m_menuBar = new QMenuBar();
  setMenuBar(m_menuBar);

  QMenu *fileMenu = new QMenu("File");
  fileMenu->addAction("New Project...");
  fileMenu->addAction("Open Project...");
  fileMenu->addAction("Save Project");
  fileMenu->addAction("Save Project As...");
  m_menuBar->addMenu(fileMenu);
}

}
