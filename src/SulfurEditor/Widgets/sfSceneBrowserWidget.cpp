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
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Components/sfTransform.hpp"
#include "Components/sfCamera.hpp"
#include "Components/sfMeshRenderer.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "SystemTable/sfSystemTable.hpp"

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
  m_scenePropertiesDialog->SetScene(scene);
  m_sceneTree->clear();
  m_itemMap.clear();

  const std::list<HNDL>& rootObjects = scene->GetRootObjects();
  for (auto it = rootObjects.cbegin(); it != rootObjects.cend(); it++)
    AddObject(*it);
}

void SceneBrowserWidget::SelectObject(Object *object)
{
  if (object)
    SelectObject(object->GetHndl());
  else
    m_sceneTree->clearSelection();
}

void SceneBrowserWidget::SelectObject(HNDL object)
{
  m_sceneTree->clearSelection();

  if (object != SF_INV_HANDLE)
    m_itemMap[object]->setSelected(true);
}

void SceneBrowserWidget::UpdateSelectedObjects()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty()) return;

  Object *object = g_SystemTable->ObjFactory->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
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
  m_newObjectButton->setMenu(newObjectMenu);

  QMenu *meshesMenu = newObjectMenu->addMenu("Meshes");
  meshesMenu->addAction("Cube", this, &SceneBrowserWidget::OnAddCube);
  meshesMenu->addAction("Plane", this, &SceneBrowserWidget::OnAddPlane);
  meshesMenu->addAction("Capsule", this, &SceneBrowserWidget::OnAddCapsule);
  meshesMenu->addAction("Cone", this, &SceneBrowserWidget::OnAddCone);
  meshesMenu->addAction("Cylinder", this, &SceneBrowserWidget::OnAddCylinder);
  meshesMenu->addAction("Sphere", this, &SceneBrowserWidget::OnAddSphere);

  QMenu *lightsMenu = newObjectMenu->addMenu("Lights");
  lightsMenu->addAction("Point Light", this, &SceneBrowserWidget::OnAddPointLight);
  lightsMenu->addAction("Spot Light", this, &SceneBrowserWidget::OnAddSpotLight);
  lightsMenu->addAction("Directional Light", this, &SceneBrowserWidget::OnAddDirectionalLight);

  m_scenePropertiesDialog = new ScenePropertiesDialog();
  m_propertiesButton = new QPushButton("Properties");
  m_layout->addWidget(m_propertiesButton, 0, 1);

  QObject::connect(
    m_propertiesButton, &QPushButton::clicked,
    this, &SceneBrowserWidget::OnPropertiesClicked
    );

  QObject::connect(
    m_sceneTree, &QTreeWidget::itemSelectionChanged,
    this, &SceneBrowserWidget::OnSceneTreeSelectionChanged
    );

  QObject::connect(
    m_sceneTree->model(), &QAbstractItemModel::rowsInserted,
    this, &SceneBrowserWidget::OnItemsMoved
    );
}

void SceneBrowserWidget::AddObject(HNDL objectHandle, QTreeWidgetItem *root)
{
  AddObject(g_SystemTable->ObjFactory->GetObject(objectHandle), root);
}

void SceneBrowserWidget::AddObject(Object *object, QTreeWidgetItem *root)
{
  if (root == nullptr) 
    root = m_sceneTree->invisibleRootItem();

  // Add object
  QTreeWidgetItem *item = new QTreeWidgetItem({ object->m_name.c_str() });
  item->setData(0, Qt::UserRole, QVariant::fromValue(object->GetHndl()));
  root->addChild(item);

  m_itemMap[object->GetHndl()] = item;

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

    g_SystemTable->ObjFactory->DestroyObject(objectHandle);
  }
}

Object* SceneBrowserWidget::CreateObjectInFrontOfCamera(const std::string& name)
{
  Object *object = g_SystemTable->ObjFactory->GetObject(m_scene->CreateObject(name));
  Transform *transform = object->GetComponent<Transform>();

  Object *cameraObject = g_SystemTable->ObjFactory->GetObject(m_scene->GetCameraObject());
  Transform *cameraTransform = cameraObject->GetComponent<Transform>();

  transform->SetTranslation(
    cameraTransform->GetTranslation() +
    cameraTransform->GetForward() * 5.0f
    );

  return object;
}

void SceneBrowserWidget::AddMeshObject(const std::string& objectName, const std::string& resourceName)
{
  Object *object = CreateObjectInFrontOfCamera(objectName);

  MeshRenderer *meshRenderer = g_SystemTable->CompFactory->CreateComponent<MeshRenderer>();
  meshRenderer->SetMesh(resourceName);

  object->AttachComponent(meshRenderer);
  AddObject(object);
  SelectObject(object);
}

void SceneBrowserWidget::AddComponentObject(const std::string& objectName, const std::string& component)
{
  Object *object = CreateObjectInFrontOfCamera(objectName);

  IEntity *componentInstance = g_SystemTable->CompFactory->CreateComponent(component);
  object->AttachComponent(componentInstance);

  AddObject(object);
  SelectObject(object);
}

void SceneBrowserWidget::OnPropertiesClicked()
{
  m_scenePropertiesDialog->show();
}

void SceneBrowserWidget::OnSceneTreeSelectionChanged()
{
  QList<QTreeWidgetItem*> selection = m_sceneTree->selectedItems();
  if (selection.empty())
    emit ObjectSelected(nullptr);
  else
  {
    Object *object = g_SystemTable->ObjFactory->GetObject(selection.front()->data(0, Qt::UserRole).value<HNDL>());
    emit ObjectSelected(object);
  }
}

void SceneBrowserWidget::OnItemsMoved(const QModelIndex &parent, int start, int end)
{
  HNDL parentHandle = SF_INV_HANDLE;
  if (!parent.data().isNull())
    parentHandle = parent.data(Qt::UserRole).value<HNDL>();

  for (int i = start; i <= end; ++i)
  {
    HNDL childHandle;
    if (parentHandle == SF_INV_HANDLE) childHandle = m_sceneTree->model()->index(i, 0).data(Qt::UserRole).value<HNDL>();
    else childHandle = parent.child(i, 0).data(Qt::UserRole).value<HNDL>();

    Object *child = g_SystemTable->ObjFactory->GetObject(childHandle);
    HNDL currentParent = child->GetParent();

    if (parentHandle != currentParent)
    {
      if (child->GetParent() == SF_INV_HANDLE)
        m_scene->RemoveFromRoot(childHandle);
      else if (parentHandle == SF_INV_HANDLE)
        m_scene->AddObject(childHandle);

      child->SetParent(parentHandle);
    }
  }
}

void SceneBrowserWidget::OnAddEmptyObject()
{
  Object *object = CreateObjectInFrontOfCamera("Game Object");

  AddObject(object);
  SelectObject(object);
}

void SceneBrowserWidget::OnAddCamera()
{
  AddComponentObject("Camera", "Camera");
}

void SceneBrowserWidget::OnAddCube()
{
  AddMeshObject("Cube", "Models/cube.fbx");  
}

void SceneBrowserWidget::OnAddPlane()
{
  AddMeshObject("Plane", "Models/plane.fbx");
}

void SceneBrowserWidget::OnAddCapsule()
{
  AddMeshObject("Capsule", "Models/capsule.fbx");
}

void SceneBrowserWidget::OnAddCone()
{
  AddMeshObject("Cone", "Models/cone.fbx");
}

void SceneBrowserWidget::OnAddCylinder()
{
  AddMeshObject("Cylinder", "Models/cylinder.fbx");
}

void SceneBrowserWidget::OnAddSphere()
{
  AddMeshObject("Sphere", "Models/sphere.fbx");
}

void SceneBrowserWidget::OnAddPointLight()
{
  AddComponentObject("Point Light", "PointLight");
}

void SceneBrowserWidget::OnAddSpotLight()
{
  AddComponentObject("Spot Light", "SpotLight");
}

void SceneBrowserWidget::OnAddDirectionalLight()
{
  AddComponentObject("Directional Light", "DirectionalLight");
}


}
