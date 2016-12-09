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
#include "Components/sfTransform.hpp"
#include "Modules/Input/sfInputManager.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent), m_controllingCamera(false)
{
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NativeWindow, true);

  Core::Instance()->StartUp((HWND)winId());
  
  Object *editorCamera = ObjectFactory::Instance()->CreateObject("EditorCamera");

  Camera *cameraComponent = ComponentFactory::Instance()->CreateComponent<Camera>();
  cameraComponent->SetProjectionType(ProjectionType::PERSPECTIVE);
  cameraComponent->SetNearPlane(0.1f);
  cameraComponent->SetFarPlane(1000.0f);
  cameraComponent->SetFieldOfView(SF_PI / 4.0f);
  editorCamera->AttachComponent(cameraComponent);

  m_editorCamera = editorCamera->GetHndl();
  SceneManager::Instance()->GetScene().SetCameraObject(m_editorCamera);
}

GameWidget::~GameWidget()
{
  Core::Instance()->ShutDown();
}

void GameWidget::Frame()
{
  UpdateEditorCamera();
  Core::Instance()->Frame();
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MouseButton::RightButton)
  {
    ShowCursor(FALSE);
    m_controllingCamera = true;
  }

  QWidget::mousePressEvent(event);
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MouseButton::RightButton)
  {
    ShowCursor(TRUE);
    m_controllingCamera = false;
  }

  QWidget::mouseReleaseEvent(event);
}

void GameWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
  QWidget::mouseDoubleClickEvent(event);
}

void GameWidget::mouseMoveEvent(QMouseEvent *event)
{
  QWidget::mouseMoveEvent(event);
}

void GameWidget::UpdateEditorCamera()
{
  Object *editorCamera = SF_GET_OBJECT(m_editorCamera);
  Transform *transform = editorCamera->GetComponent<Transform>();
  transform->Update();

  if (m_controllingCamera)
  {
    InputManager *inputManager = InputManager::Instance();

    Quaternion rotation = transform->GetRotation();
    rotation = Quaternion(Vector3::c_yAxis, inputManager->MouseDeltaX() * -0.01f) * rotation;
    rotation = Quaternion(transform->GetRight(), inputManager->MouseDeltaY() * -0.01f) * rotation;

    // Camera rotation
    transform->SetRotation(
      Quaternion(transform->GetRight(), inputManager->MouseDeltaY() * -0.01f) *
      Quaternion(Vector3::c_yAxis, inputManager->MouseDeltaX() * -0.01f) *
      transform->GetRotation());

    // Camera movement
    transform->SetTranslation(
      transform->GetTranslation() +
      transform->GetUp() * (inputManager->IsKeyDown('E') - inputManager->IsKeyDown('Q')) * 0.001f +
      transform->GetRight() * (inputManager->IsKeyDown('D') - inputManager->IsKeyDown('A')) * 0.001f +
      transform->GetForward() * (inputManager->IsKeyDown('W') - inputManager->IsKeyDown('S')) * 0.001f
      );
  }
}

}
