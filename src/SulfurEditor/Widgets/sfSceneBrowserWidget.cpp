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
#include "Factories/sfComponentFactory.hpp"
#include "Components/sfCamera.hpp"
#include "Components/sfMeshRenderer.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"

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
    AddObject(object);
}

void SceneBrowserWidget::UpdateSelectedObjects()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty()) return;

  Object *object = ObjectFactory::Instance()->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
  selection.front()->setText(0, object->m_name.c_str());
}

void SceneBrowserWidget::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
  case Qt::Key_Delete:
    DeleteSelectedObjects();
    break;

  default:
    event->ignore();
    return;
  }
}

void SceneBrowserWidget::Setup()
{
  setContentsMargins(0, 0, 0, 0);
  setMinimumSize(350, 175);
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);

  m_layout = new QGridLayout();
  m_layout->setMargin(0);
  setLayout(m_layout);

  m_sceneTree = new QTreeWidget();
  m_sceneTree->setHeaderHidden(true);
  m_sceneTree->setDragEnabled(true);
  m_sceneTree->setDragDropMode(QAbstractItemView::InternalMove);
  m_layout->addWidget(m_sceneTree, 1, 0, 1, 2);

  m_newObjectButton = new QToolButton();
  m_newObjectButton->setMinimumWidth(100);
  m_newObjectButton->setText("New");
  m_newObjectButton->setPopupMode(QToolButton::InstantPopup);
  m_layout->addWidget(m_newObjectButton, 0, 0);

  QMenu *newObjectMenu = new QMenu();
  newObjectMenu->addAction("Empty Object", this, &SceneBrowserWidget::OnAddEmptyObject);
  newObjectMenu->addAction("Camera", this, &SceneBrowserWidget::OnAddCamera);
  newObjectMenu->addAction("Cube", this, &SceneBrowserWidget::OnAddCube);
  m_newObjectButton->setMenu(newObjectMenu);

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
  AddObject(ObjectFactory::Instance()->GetObject(objectHandle), root);
}

void SceneBrowserWidget::AddObject(Object *object, QTreeWidgetItem *root)
{
  if (root == nullptr) root = m_sceneTree->invisibleRootItem();

  // Add object
  QTreeWidgetItem *item = new QTreeWidgetItem({ object->m_name.c_str() });
  item->setData(0, Qt::UserRole, QVariant::fromValue(object->GetHndl()));
  root->addChild(item);

  // Add children
  auto& children = object->GetChildren();
  for (auto& child : children)
    AddObject(child, item);
}

void SceneBrowserWidget::DeleteSelectedObjects()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  for (QTreeWidgetItem *item : selection)
  {
    HNDL objectHandle = selection.front()->data(0, Qt::UserRole).value<HNDL>();
    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr)
    {
      m_scene->RemoveFromRoot(objectHandle);
      m_sceneTree->invisibleRootItem()->removeChild(item);
    }
    else
    {
      parent->removeChild(item);
    }

    ObjectFactory::Instance()->DestroyObject(objectHandle);
  }
}

void SceneBrowserWidget::OnSceneTreeSelectionChanged()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty())
    emit ObjectSelected(nullptr);
  else
  {
    Object *object = ObjectFactory::Instance()->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
    emit ObjectSelected(object);
  }
}

void SceneBrowserWidget::OnItemInserted(const QModelIndex& parent, int start, int end)
{
  HNDL parentHandle = SF_INV_HANDLE;
  if (!parent.data().isNull())
    parentHandle = parent.data(Qt::UserRole).value<HNDL>();

  for (int i = start; i <= end; ++i)
  {
    HNDL childHandle = parent.child(i, 0).data(Qt::UserRole).value<HNDL>();
    Object *child = ObjectFactory::Instance()->GetObject(childHandle);

    if (child->GetParent() == SF_INV_HANDLE)
      m_scene->RemoveFromRoot(childHandle);
    if (parentHandle == SF_INV_HANDLE)
      m_scene->AddObject(childHandle);

    child->SetParent(parentHandle);
  }
}

void SceneBrowserWidget::OnAddEmptyObject()
{
  Object *object = ObjectFactory::Instance()->GetObject(m_scene->CreateObject("Game Object"));
  AddObject(object);
}

void SceneBrowserWidget::OnAddCamera()
{
  Object *object = ObjectFactory::Instance()->GetObject(m_scene->CreateObject("Camera"));

  Camera *camera = ComponentFactory::Instance()->CreateComponent<Camera>();
  object->AttachComponent(camera);

  AddObject(object);
}

void SceneBrowserWidget::OnAddCube()
{
  Object *object = ObjectFactory::Instance()->GetObject(m_scene->CreateObject("Cube"));

  MeshRenderer *meshRenderer = ComponentFactory::Instance()->CreateComponent<MeshRenderer>();
  meshRenderer->SetMesh("Models/cube.fbx");

  object->AttachComponent(meshRenderer);
  AddObject(object);
}

}
