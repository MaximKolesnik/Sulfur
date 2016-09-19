/******************************************************************************/
/*!
\par     Sulfur
\file    sfSceneBrowserWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Scene browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfSceneBrowserWidget.hpp"
#include "Factories/sfObjectFactory.hpp"

namespace Sulfur
{

SceneBrowserWidget::SceneBrowserWidget(QWidget *parent)
  : QWidget(parent)
{
  Setup();
}

SceneBrowserWidget::SceneBrowserWidget(Scene *scene, QWidget *parent)
  : QWidget(parent)
{
  Setup();
  SetScene(scene);
}

SceneBrowserWidget::~SceneBrowserWidget()
{
}

void SceneBrowserWidget::SetScene(Scene *scene)
{
  m_scene = scene;
  m_sceneTree->clear();

  auto& rootObjects = scene->GetRootObjects();
  for (HNDL object : rootObjects)
    AddObject(object, m_sceneTree->invisibleRootItem());
}

void SceneBrowserWidget::Setup()
{
  setContentsMargins(0, 0, 0, 0);

  m_layout = new QGridLayout();
  m_layout->setMargin(0);
  setLayout(m_layout);

  m_sceneTree = new QTreeWidget();
  m_sceneTree->setHeaderHidden(true);
  m_layout->addWidget(m_sceneTree);
}

void SceneBrowserWidget::AddObject(HNDL objectHandle, QTreeWidgetItem *root)
{
  Object *object = ObjectFactory::Instance()->GetObject(objectHandle);

  // Add object
  QTreeWidgetItem *item = new QTreeWidgetItem({ object->m_name.c_str() });
  root->addChild(item);

  // Add children
  auto& children = object->GetChildren();
  for (auto& child : children)
    AddObject(child.first, item);
}

}
