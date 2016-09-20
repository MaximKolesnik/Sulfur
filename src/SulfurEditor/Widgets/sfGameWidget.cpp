/******************************************************************************/
/*!
\par     Sulfur
\file    sfGameWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Game rendering widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGameWidget.hpp"
#include "Core/sfCore.hpp"

#include "Modules/Scene/sfSceneManager.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Components/sfCamera.hpp"
#include "Components/sfMeshRenderer.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent)
{
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NativeWindow, true);

  Core::Instance()->StartUp((HWND)winId());

  Scene& scene = SceneManager::Instance()->GetScene();

  HNDL root0 = scene.CreateObject("Camera");
  Object *object = ObjectFactory::Instance()->GetObject(root0);
  object->AttachComponent(ComponentFactory::Instance()->CreateComponent<Camera>());

  MeshRenderer *renderer = ComponentFactory::Instance()->CreateComponent<MeshRenderer>();
  renderer->SetMesh(new Mesh());
  renderer->GetMesh()->Init(GraphicsManager::Instance()->GetDevice());
  renderer->GetMesh()->CreateBox(100.0f, 100.0f, 100.0f);
  object->AttachComponent(renderer);

  HNDL sub0 = scene.CreateObject("Sub 0", root0);
  HNDL sub1 = scene.CreateObject("Sub 1", root0);
  HNDL sub2 = scene.CreateObject("Sub 2", root0);
  HNDL sub3 = scene.CreateObject("Sub 3", root0);

  HNDL root1 = scene.CreateObject("Root 1");
  HNDL sub4 = scene.CreateObject("Sub 4", root1);
  HNDL sub5 = scene.CreateObject("Sub 5", root1);
  HNDL sub6 = scene.CreateObject("Sub 6", root1);

  HNDL root2 = scene.CreateObject("Root 2");
  HNDL sub7 = scene.CreateObject("Sub 7", root2);

  HNDL root3 = scene.CreateObject("Root 3");
  HNDL sub8 = scene.CreateObject("Sub 8", root3);
  HNDL sub9 = scene.CreateObject("Sub 9", root3);
  HNDL sub10 = scene.CreateObject("Sub 10", root3);

  HNDL root4 = scene.CreateObject("Root 4");
  HNDL sub11 = scene.CreateObject("Sub 11", root4);
  HNDL sub12 = scene.CreateObject("Sub 12", root4);

}

GameWidget::~GameWidget()
{
  Core::Instance()->ShutDown();
}

void GameWidget::Frame()
{
  Core::Instance()->Frame();
}

}
