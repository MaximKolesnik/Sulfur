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

  public slots:
    void OnSceneTreeSelectionChanged();
    void OnItemMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void OnAddEmptyObject();
    void OnAddCamera();
    void OnAddCube();
    void OnAddPlane();

  signals:
    void ObjectSelected(Object *object);

  private:
    Scene *m_scene;

    QGridLayout *m_layout;
    QToolButton *m_newObjectButton;
    QTreeWidget *m_sceneTree;
    std::unordered_map<HNDL, QTreeWidgetItem*> m_itemMap;
  
  };
  
}