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
  showMaximized();

  QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
  outputDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_output = new OutputWidget(outputDock);
  outputDock->setWidget(m_output);

  addDockWidget(Qt::BottomDockWidgetArea, outputDock);

  m_game = new GameWidget();
  setCentralWidget(m_game);

  QDockWidget *inspectorDock = new QDockWidget(tr("Inspector"), this);
  inspectorDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_inspector = new InspectorWidget(inspectorDock);
  inspectorDock->setWidget(m_inspector);

  QObject::connect(
    m_inspector, &InspectorWidget::ObjectChanged,
    this, &EditorWindow::OnObjectChanged
    );

  addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

  QDockWidget *sceneDock = new QDockWidget(tr("Scene"), this);
  sceneDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_sceneBrowser = new SceneBrowserWidget(sceneDock);
  m_sceneBrowser->SetScene(&SceneManager::Instance()->GetScene());
  sceneDock->setWidget(m_sceneBrowser);

  QObject::connect(
    m_sceneBrowser, &SceneBrowserWidget::ObjectSelected,
    this, &EditorWindow::OnObjectSelected
    );

  addDockWidget(Qt::LeftDockWidgetArea, sceneDock);

  QDockWidget *resourceBrowserDock = new QDockWidget(tr("Resources"), this);
  resourceBrowserDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_resourceBrowser = new ResourceBrowserWidget(resourceBrowserDock);
  m_resourceBrowser->SetProjectRoot(QDir::currentPath());
  resourceBrowserDock->setWidget(m_resourceBrowser);

  addDockWidget(Qt::BottomDockWidgetArea, resourceBrowserDock);

  
  tabifyDockWidget(resourceBrowserDock, outputDock);
  resourceBrowserDock->raise();

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

void EditorWindow::OnObjectSelected(Object *object)
{
  m_inspector->SetObject(object);
}

void EditorWindow::OnObjectChanged()
{
  m_sceneBrowser->UpdateSelectedObjects();
}

}
