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
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Time/sfTime.hpp"
#include "Modules/Graphics/Resources/Buffer/sfBufferData.hpp"
#include "Components/sfMeshRenderer.hpp"

#include "Modules/Graphics/State/sfBlendState.hpp"
#include "Modules/Graphics/State/sfDepthState.hpp"
#include "Modules/Graphics/State/sfRasterState.hpp"
#include "Modules/Graphics/State/sfSamplerState.hpp"
#include "Modules\Graphics\Debug\sfDebugDraw.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent), m_controllingCamera(false), m_cameraYaw(0.0f), m_cameraPitch(0.0f), m_resizeTimer(0),
  m_selection(nullptr)
{
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NativeWindow, true);

  Core::Instance()->StartUp((HWND)winId());
  m_window = Core::Instance()->GetWindow();

  Object *editorCamera = ObjectFactory::Instance()->CreateObject("EditorCamera");

  Camera *cameraComponent = ComponentFactory::Instance()->CreateComponent<Camera>();
  cameraComponent->SetProjectionType(ProjectionType::PERSPECTIVE);
  cameraComponent->SetNearPlane(0.1f);
  cameraComponent->SetFarPlane(1000.0f);
  cameraComponent->SetFieldOfView(45.0f);
  editorCamera->AttachComponent(cameraComponent);

  m_editorCamera = editorCamera->GetHndl();
  SceneManager::Instance()->GetScene().SetCameraObject(m_editorCamera);

  CreatePickingResources();
}

GameWidget::~GameWidget()
{
  m_pickingTarget.Free();
  m_pickingDepthBuffer.Free();
  m_stagingTexture->Release();
  Core::Instance()->ShutDown();
}

void GameWidget::Frame()
{
  SelectionDrawing();
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

  setFocus(Qt::FocusReason::OtherFocusReason);
  QWidget::mousePressEvent(event);
}

void GameWidget::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MouseButton::RightButton)
  {
    ShowCursor(TRUE);
    m_controllingCamera = false;
  }
  else if (event->button() == Qt::MouseButton::LeftButton)
  {
    RenderPickingTexture();
    SelectObjectAt(event->x(), event->y());
  }

  setFocus(Qt::FocusReason::OtherFocusReason);
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

void GameWidget::CreatePickingResources()
{
  D3D11_TEXTURE2D_DESC description;
  description.Width = 1024;
  description.Height = 1024;
  description.MipLevels = 1;
  description.ArraySize = 1;
  description.Format = DXGI_FORMAT_R32_UINT;
  description.SampleDesc.Count = 1;
  description.SampleDesc.Quality = 0;
  description.Usage = D3D11_USAGE_DEFAULT;
  description.BindFlags = D3D11_BIND_RENDER_TARGET;
  description.CPUAccessFlags = 0;
  description.MiscFlags = 0;
  m_pickingTarget.Init(GraphicsManager::Instance()->GetDevice(), description);

  description.Usage = D3D11_USAGE_STAGING;
  description.BindFlags = 0;
  description.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
  GraphicsManager::Instance()->GetDevice().GetD3DResource()->CreateTexture2D(&description, nullptr, &m_stagingTexture);

  description.Usage = D3D11_USAGE_DEFAULT;
  description.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  description.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  description.CPUAccessFlags = 0;
  m_pickingDepthBuffer.Init(GraphicsManager::Instance()->GetDevice(), description);

  m_pickingVertexShader.Init(GraphicsManager::Instance()->GetDevice(), "Shaders/VSTest.sbin");
  m_perFrameData = m_pickingVertexShader.GetConstantBuffer("PerFrameData");
  m_perObjectData = m_pickingVertexShader.GetConstantBuffer("PerObjectData");

  m_pickingPixelShader.Init(GraphicsManager::Instance()->GetDevice(), "Shaders/PSPicking.sbin");
  m_pickingData = m_pickingPixelShader.GetConstantBuffer("PickingData");
}

void GameWidget::RenderPickingTexture()
{
  D3D11Context& context = GraphicsManager::Instance()->GetDevice().GetImmediateContext();

  m_pickingTarget.Clear(context, Vector4(0.0f, 0.0f, 0.0f, 0.0f));
  m_pickingDepthBuffer.Clear(context);
  m_pickingTarget.Set(context, m_pickingDepthBuffer);

  DepthState::Set(context, DepthState::DEPTH_ENABLED);
  BlendState::Set(context, BlendState::NO_BLENDING);
  RasterState::Set(context, RasterState::BACK_FACE_CULLING);

  m_pickingVertexShader.Set(context);
  m_pickingPixelShader.Set(context);

  Scene& scene = SceneManager::Instance()->GetScene();
  
  // Setup camera
  HNDL objHandle = scene.GetCameraObject();
  if (objHandle != SF_INV_HANDLE)
  {
    Object *object = SF_GET_OBJECT(scene.GetCameraObject());
    Transform *transform = object->GetComponent<Transform>();
    Camera *camera = object->GetComponent<Camera>();

    PerFrameData perFrame;
    perFrame.ViewMatrix.SetViewMatrix(transform->GetWorldRight(), transform->GetWorldUp(), transform->GetWorldForward(), transform->GetWorldTranslation());
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)width(), (Real)height(), camera->GetFieldOfView() * SF_RADS_PER_DEG, camera->GetNearPlane(), camera->GetFarPlane());
    perFrame.ViewPosition = transform->GetWorldTranslation();
    m_perFrameData->SetData(context, perFrame);
  }
  else
  {
    PerFrameData perFrame;
    perFrame.ViewMatrix.SetLookAtLH(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f));
    perFrame.ProjMatrix.SetPerspectiveFovLH((Real)width(), (Real)height(), 3.14159f / 4.0f, 0.1f, 1000.0f);
    perFrame.ViewPosition = Vector3(0.0f, 0.0f, 0.0f);
    m_perFrameData->SetData(context, perFrame);
  }

  ComponentFactory::ComponentData componentData = ComponentFactory::Instance()->GetComponentData<MeshRenderer>();
  for (auto it = componentData.begin(); it != componentData.end(); ++it)
  {
    MeshRenderer *meshRenderer = static_cast<MeshRenderer*>(*it);
    Mesh *mesh = meshRenderer->GetMesh();

    if (mesh)
    {
      Object *object = SF_GET_OBJECT(meshRenderer->GetOwner());
      Transform* transform = object->GetComponent<Transform>();

      PerObjectData perObject;
      perObject.WorldMatrix = transform->GetWorldMatrix();
      m_perObjectData->SetData(context, perObject);

      UINT32 handle = (UINT32)meshRenderer->GetOwner() + 1;
      m_pickingData->SetData(context, handle);

      mesh->Draw(context);
    }
  }
}

void GameWidget::SelectObjectAt(int x, int y)
{
  int px = (int)((Real)x / width() * 1024);
  int py = (int)((Real)y / height() * 1024);

  ID3D11DeviceContext *context = GraphicsManager::Instance()->GetDevice().GetImmediateContext().GetD3DResource();
  context->CopyResource(m_stagingTexture, m_pickingTarget.GetTexture()->GetD3DResource());

  D3D11_MAPPED_SUBRESOURCE msr;
  context->Map(m_stagingTexture, 0, D3D11_MAP_READ, 0, &msr);

  UINT32 *pickingData = (UINT32*)msr.pData;
  UINT32 handle = pickingData[py * 1024 + px];

  if (handle != 0)
    emit ObjectSelected(SF_GET_OBJECT((HNDL)handle-1));
  else
    emit ObjectSelected(nullptr);

  context->Unmap(m_stagingTexture, 0);
}

void GameWidget::SelectionDrawing()
{
  if (m_selection)
  {
    m_selection->DrawDebug(DebugDraw::Instance());

    auto& components = m_selection->GetComponents();
    for (auto it = components.begin(); it != components.end(); ++it)
    {
      ComponentFactory::Instance()->GetComponent(it->first, it->second)->DrawDebug(DebugDraw::Instance());
    }
  }
}

void GameWidget::UpdateEditorCamera()
{
  Object *editorCamera = SF_GET_OBJECT(m_editorCamera);
  Transform *transform = editorCamera->GetComponent<Transform>();
  transform->Update();

  if (m_controllingCamera)
  {
    InputManager *inputManager = InputManager::Instance();

    // Camera rotation
    Real rotationAmount = 50.0f * Time::Instance()->GetDt();
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
    Real translationAmount = (1.0f + inputManager->IsKeyDown(VK_SHIFT)) * Time::Instance()->GetDt();
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

void GameWidget::SetSelection(Object *object)
{
  m_selection = object;
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
