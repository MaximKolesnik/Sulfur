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
#include "Modules/Time/sfTime.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent), m_controllingCamera(false), m_cameraYaw(0.0f), m_cameraPitch(0.0f), m_resizeTimer(0)
{
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NativeWindow, true);

  Core::Instance()->StartUp((HWND)winId());
  m_window = Core::Instance()->GetWindow();

  Object *editorCamera = g_SystemTable->ObjFactory->CreateObject("EditorCamera");

  Camera *cameraComponent = g_SystemTable->CompFactory->CreateComponent<Camera>();
  cameraComponent->SetProjectionType(ProjectionType::PERSPECTIVE);
  cameraComponent->SetNearPlane(0.1f);
  cameraComponent->SetFarPlane(1000.0f);
  cameraComponent->SetFieldOfView(45.0f);
  editorCamera->AttachComponent(cameraComponent);

  m_editorCamera = editorCamera->GetHndl();
  g_SystemTable->SceneManager->GetScene().SetCameraObject(m_editorCamera);
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
  Object *editorCamera = g_SystemTable->ObjFactory->GetObject(m_editorCamera);
  Transform *transform = editorCamera->GetComponent<Transform>();
  transform->Update();

  if (m_controllingCamera)
  {
    InputManager *inputManager = g_SystemTable->InputManager;

    // Camera rotation
    Real rotationAmount = 50.0f * g_SystemTable->Time->GetDt();
    m_cameraYaw += inputManager->MouseDeltaX() * -rotationAmount;
    m_cameraPitch += inputManager->MouseDeltaY() * -rotationAmount;
    while (m_cameraYaw < 0.0f) m_cameraYaw += 360.0f;
    while (m_cameraYaw >= 360.0f) m_cameraYaw -= 360.0f;
    if (m_cameraPitch < -90.0f) m_cameraPitch = -90.0f;
    if (m_cameraPitch > 90.0f) m_cameraPitch = 90.0f;

    Quaternion rotation;
    rotation.SetEuler(m_cameraPitch * SF_RADS_PER_DEG, 0.0f, m_cameraYaw * SF_RADS_PER_DEG);
    transform->SetRotation(rotation);

    // Camera movement
    Real translationAmount = (1.0f + inputManager->IsKeyDown(VK_SHIFT)) * g_SystemTable->Time->GetDt();
    transform->SetTranslation(
      transform->GetTranslation() +
      transform->GetUp() * (inputManager->IsKeyDown('E') - inputManager->IsKeyDown('Q')) * translationAmount +
      transform->GetRight() * (inputManager->IsKeyDown('D') - inputManager->IsKeyDown('A')) * translationAmount +
      transform->GetForward() * (inputManager->IsKeyDown('W') - inputManager->IsKeyDown('S')) * translationAmount
      );
  }
}

QPaintEngine* GameWidget::paintEngine() const
{
  return nullptr;
}

void GameWidget::resizeEvent(QResizeEvent* evt)
{
  if (m_resizeTimer)
    killTimer(m_resizeTimer);

  m_resizeTimer = startTimer(250);
}

void GameWidget::paintEvent(QPaintEvent* evt)
{
}

void GameWidget::timerEvent(QTimerEvent* evt)
{
  if (evt->timerId() == m_resizeTimer)
  {
    m_window->OnSize(width(), height());

    killTimer(m_resizeTimer);
    m_resizeTimer = 0;
  }
}


}
