/******************************************************************************/
/*!
\par     Sulfur
\file    sfScenePropertiesDialog.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/9/2016

\brief   Scene properties dialog window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Scene/sfScene.hpp"
#include "Widgets/sfInspectorWidget.hpp"

namespace Sulfur
{
	
  class ScenePropertiesDialog : public QDialog
  {
    Q_OBJECT

  public:
    ScenePropertiesDialog(QWidget *parent = 0);
    ~ScenePropertiesDialog();

    void SetScene(Scene *scene);

  private:
    QBoxLayout *m_layout;
    InspectorWidget *m_inspector;
    Scene *m_scene;
  
  };
  
}