/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWindow.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Main editor window

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfEditorWindow.hpp"
#include "Widgets/sfGameWidget.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"

namespace Sulfur
{

EditorWindow::EditorWindow(QWidget *parent)
  : QMainWindow(parent)
{
  resize(1920, 1080);
  showMaximized();
  setWindowTitle("Sulfur Editor");

  m_aboutWindow = new AboutWindow();

  QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
  outputDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_output = new OutputWidget(outputDock);
  outputDock->setWidget(m_output);

  addDockWidget(Qt::BottomDockWidgetArea, outputDock);

  m_editor = new EditorWidget();
  setCentralWidget(m_editor);

  QObject::connect(
    m_editor->GetGameWidget(), &GameWidget::ObjectSelected,
    this, &EditorWindow::OnObjectSelectedGameWindow
    );

  QObject::connect(
    m_editor, &EditorWidget::DeleteSelection,
    this, &EditorWindow::OnDeleteSelection
    );

  QObject::connect(
    m_editor, &EditorWidget::DuplicateSelection,
    this, &EditorWindow::OnDuplicateSelection
    );

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

  QObject::connect(
    m_sceneBrowser, &SceneBrowserWidget::ObjectActivated,
    this, &EditorWindow::OnObjectActivated
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
  m_editor->GetGameWidget()->Frame();
}

void EditorWindow::CreateMenuBar()
{
  m_menuBar = new QMenuBar();
  setMenuBar(m_menuBar);

  QMenu *fileMenu = new QMenu("File");
  fileMenu->addAction("New Scene", this, &EditorWindow::OnNewScene);
  fileMenu->addAction("Open Scene", this, &EditorWindow::OnOpenScene);
  fileMenu->addAction("Save Scene", this, &EditorWindow::OnSaveScene);
  fileMenu->addAction("Save Scene As...", this, &EditorWindow::OnSaveSceneAs);
  m_menuBar->addMenu(fileMenu);

  QMenu *viewMenu = new QMenu("View");
  m_debugDrawAction = viewMenu->addAction("Debug Draw", this, &EditorWindow::OnDebugDraw);
  m_debugDrawAction->setCheckable(true);
  m_debugDrawAction->setChecked(DebugDraw::Instance()->IsEnabled());
  m_menuBar->addMenu(viewMenu);

  QMenu *helpMenu = new QMenu("Help");
  helpMenu->addAction("About", this, &EditorWindow::OnAbout);
  m_menuBar->addMenu(helpMenu);
}

void EditorWindow::OnNewScene()
{
  SceneManager::Instance()->GetScene() = Scene();
  ObjectFactory::Instance()->DestroyAll();
  m_sceneBrowser->SetScene(&SceneManager::Instance()->GetScene());
  m_editor->GetGameWidget()->SetupEditor();
}

void EditorWindow::OnOpenScene()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Open Scene", QDir::currentPath() + "/Resources", "Sulfur Scene (*.ssc)");
  if (!fileName.isEmpty())
  {
    std::ifstream file(fileName.toUtf8().data(), std::ios_base::binary);
    Serialization::Deserialize(file, SceneManager::Instance()->GetScene());
    m_sceneBrowser->SetScene(&SceneManager::Instance()->GetScene());
    m_editor->GetGameWidget()->SetupEditor();
  }
}

void EditorWindow::OnSaveScene()
{
  OnSaveSceneAs();
}

void EditorWindow::OnSaveSceneAs()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save Scene", QDir::currentPath() + "/Resources", "Sulfur Scene (*.ssc)");
  if (!fileName.isEmpty())
  {
    std::ofstream file(fileName.toUtf8().data(), std::ios_base::binary);
    Serialization::Serialize(file, SceneManager::Instance()->GetScene());
  }
}

void EditorWindow::OnDebugDraw()
{
  DebugDraw::Instance()->SetEnabled(!DebugDraw::Instance()->IsEnabled());
  m_debugDrawAction->setChecked(DebugDraw::Instance()->IsEnabled());
}

void EditorWindow::OnObjectActivated(Object *object)
{
  m_editor->GetGameWidget()->MoveToObject(object);
}

void EditorWindow::OnObjectSelected(Object *object)
{
  m_inspector->SetObject(object);
  m_editor->GetGameWidget()->SetSelection(object);
}

void EditorWindow::OnObjectSelectedGameWindow(Object *object)
{
  m_inspector->SetObject(object);
  m_sceneBrowser->SelectObject(object);
}

void EditorWindow::OnObjectChanged()
{
  m_sceneBrowser->UpdateSelectedObjects();
}

void EditorWindow::OnAbout()
{
  m_aboutWindow->show();
}

void EditorWindow::OnDeleteSelection()
{
  m_sceneBrowser->DeleteSelectedObjects();
}

void EditorWindow::OnDuplicateSelection()
{
  m_sceneBrowser->DuplicateSelectedObjects();
}

}
