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

void SceneBrowserWidget::UpdateSelectedObjects()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty()) return;

  Object *object = ObjectFactory::Instance()->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
  selection.front()->setText(0, object->m_name.c_str());
}

void SceneBrowserWidget::Setup()
{
  setContentsMargins(0, 0, 0, 0);

  m_layout = new QGridLayout();
  m_layout->setMargin(0);
  setLayout(m_layout);

  m_sceneTree = new QTreeWidget();
  m_sceneTree->setHeaderHidden(true);
  m_sceneTree->setDragEnabled(true);
  m_sceneTree->setDragDropMode(QAbstractItemView::InternalMove);
  m_layout->addWidget(m_sceneTree);

  QObject::connect(
    m_sceneTree, &QTreeWidget::itemSelectionChanged,
    this, &SceneBrowserWidget::OnSceneTreeSelectionChanged
    );

  QObject::connect(
    m_sceneTree->model(), &QAbstractItemModel::rowsInserted,
    this, &SceneBrowserWidget::OnItemInserted
    );
}

void SceneBrowserWidget::AddObject(HNDL objectHandle, QTreeWidgetItem *root)
{
  Object *object = ObjectFactory::Instance()->GetObject(objectHandle);

  // Add object
  QTreeWidgetItem *item = new QTreeWidgetItem({ object->m_name.c_str() });
  item->setData(0, Qt::UserRole, QVariant::fromValue(objectHandle));
  root->addChild(item);

  // Add children
  auto& children = object->GetChildren();
  for (auto& child : children)
    AddObject(child, item);
}

void SceneBrowserWidget::OnSceneTreeSelectionChanged()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty())
    emit ObjectSelected(nullptr);

  Object *object = ObjectFactory::Instance()->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
  emit ObjectSelected(object);
}

void SceneBrowserWidget::OnItemInserted(const QModelIndex& parent, int start, int end)
{
  if (parent.data().isNull()) return;

  HNDL parentHandle = parent.data(Qt::UserRole).value<HNDL>();

  for (int i = start; i <= end; ++i)
  {
    HNDL childHandle = parent.child(i, 0).data(Qt::UserRole).value<HNDL>();
    ObjectFactory::Instance()->GetObject(childHandle)->SetParent(parentHandle);
  }
}

}
