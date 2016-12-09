/******************************************************************************/
/*!
\par     Sulfur
\file    sfScenePropertiesDialog.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/9/2016

\brief   Scene properties dialog window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScenePropertiesDialog.hpp"

namespace Sulfur
{

  ScenePropertiesDialog::ScenePropertiesDialog(QWidget *parent)
    : QDialog(parent), m_scene(nullptr)
  {
    setModal(false);
    setWindowTitle("Scene Properties");
    setWindowFlags(Qt::WindowStaysOnTopHint);

    m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    setLayout(m_layout);

    m_inspector = new InspectorWidget();
    m_layout->addWidget(m_inspector);
  }

  ScenePropertiesDialog::~ScenePropertiesDialog()
  {

  }

  void ScenePropertiesDialog::SetScene(Scene *scene)
  {
    m_scene = scene;
    m_inspector->SetObject(&scene->m_sceneProperties);
  }


}
