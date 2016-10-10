/******************************************************************************/
/*!
\par     Sulfur
\file    sfSceneBrowserWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Scene browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Scene/sfScene.hpp"
#include "Windows/sfScenePropertiesDialog.hpp"

namespace Sulfur
{
  class Object;	

  class SceneBrowserWidget : public QWidget
  {
    Q_OBJECT

  public:
    SceneBrowserWidget(QWidget *parent = 0);
    SceneBrowserWidget(Scene *scene, QWidget *parent = 0);
    ~SceneBrowserWidget();

    void SetScene(Scene *scene);

    void SelectObject(Object *object);
    void SelectObject(HNDL object);
    void UpdateSelectedObjects();

  protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

  private:
    void Setup();
    void AddObject(HNDL objectHandle, QTreeWidgetItem *root = nullptr);
    void AddObject(Object *object, QTreeWidgetItem *root = nullptr);
    void DeleteSelectedObjects();
    Object* CreateObjectInFrontOfCamera(const std::string& name);
    void AddMeshObject(const std::string& objectName, const std::string& resourceName);
    void AddComponentObject(const std::string& objectName, const std::string& component);

  public slots:
    void OnPropertiesClicked();
    void OnSceneTreeSelectionChanged();
    void OnItemsMoved(const QModelIndex &parent, int start, int end);

    void OnAddEmptyObject();
    void OnAddCamera();
    void OnAddCube();
    void OnAddPlane();
    void OnAddCapsule();
    void OnAddCone();
    void OnAddCylinder();
    void OnAddSphere();
    void OnAddPointLight();
    void OnAddSpotLight();
    void OnAddDirectionalLight();

  signals:
    void ObjectSelected(Object *object);

  private:
    ScenePropertiesDialog *m_scenePropertiesDialog;
    Scene *m_scene;

    QGridLayout *m_layout;
    QToolButton *m_newObjectButton;
    QPushButton *m_propertiesButton;
    QTreeWidget *m_sceneTree;
    std::unordered_map<HNDL, QTreeWidgetItem*> m_itemMap;
  
  };
  
}