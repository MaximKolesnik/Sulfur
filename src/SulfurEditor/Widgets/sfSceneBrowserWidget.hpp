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
	
  class SceneBrowserWidget : public QWidget
  {
    Q_OBJECT

  public:
    SceneBrowserWidget(QWidget *parent = 0);
    SceneBrowserWidget(Scene *scene, QWidget *parent = 0);
    ~SceneBrowserWidget();

    void SetScene(Scene *scene);

    void UpdateSelectedObjects();

  protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

  private:
    void Setup();
    void AddObject(HNDL objectHandle, QTreeWidgetItem *root = nullptr);
    void AddObject(Object *object, QTreeWidgetItem *root = nullptr);
    void DeleteSelectedObjects();

  public slots:
    void OnSceneTreeSelectionChanged();
    void OnItemInserted(const QModelIndex& parent, int start, int end);
    void OnAddEmptyObject();
    void OnAddCamera();
    void OnAddCube();

  signals:
    void ObjectSelected(Object *object);

  private:
    Scene *m_scene;

    QGridLayout *m_layout;
    QToolButton *m_newObjectButton;
    QTreeWidget *m_sceneTree;
  
  };
  
}