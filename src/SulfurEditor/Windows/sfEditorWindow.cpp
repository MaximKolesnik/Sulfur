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

namespace Sulfur
{

EditorWindow::EditorWindow(QWidget *parent)
  : QMainWindow(parent)
{
  resize(1920, 1080);

  const TypeInfo *typeInfo = SF_TYPE_INFO(Transform);

  m_transform.SetTranslation(Vector3(0.0f, 100.0f, 0.0f));
  m_transform.SetRotationEulerXZY(0.0f, 0.0f, 0.0f);
  m_transform.SetScale(Vector3(1.0f, 1.0f, 1.0f));
  m_transform.m_name = "Transform";

  m_game = new GameWidget();
  setCentralWidget(m_game);

  QDockWidget *inspectorDock = new QDockWidget(tr("Inspector"), this);
  inspectorDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_inspector = new InspectorWidget(inspectorDock);
  m_inspector->SetObject(&m_transform);
  inspectorDock->setWidget(m_inspector);

  addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

  QDockWidget *resourceBrowserDock = new QDockWidget(tr("Resources"), this);
  resourceBrowserDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_resourceBrowser = new ResourceBrowserWidget(resourceBrowserDock);
  resourceBrowserDock->setWidget(m_resourceBrowser);

  addDockWidget(Qt::BottomDockWidgetArea, resourceBrowserDock);

  QDockWidget *outputDock = new QDockWidget(tr("Output"), this);
  outputDock->setAllowedAreas(Qt::DockWidgetArea_Mask);

  m_output = new OutputWidget(outputDock);
  outputDock->setWidget(m_output);

  addDockWidget(Qt::BottomDockWidgetArea, outputDock);
  tabifyDockWidget(resourceBrowserDock, outputDock);
  resourceBrowserDock->raise();

  setCorner(Qt::Corner::BottomRightCorner, Qt::DockWidgetArea::RightDockWidgetArea);
}

EditorWindow::~EditorWindow()
{
}

void EditorWindow::Frame()
{
  m_game->Frame();
}

}
