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
#include "Modules/Graphics/Utils/sfGraphicsUtils.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"

#include "Math/Geometry/sfGeometry.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent), m_controllingCamera(false), m_cameraYaw(0.0f), m_cameraPitch(0.0f), m_resizeTimer(0),
  m_selection(nullptr), m_currentGizmo(TRANSLATION_GIZMO), m_usingGizmo(false), m_state(STOPPED)
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

  Time::Instance()->SetPaused(true);
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
  if (m_state != PLAYING)
  {
    RenderPickingTexture();
    SelectionDrawing();
    UpdateEditorCamera();
  }

  Core::Instance()->Frame();
  //Physics::PhysicsWorld::Instance()->DebugDrawColliders();
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::MouseButton::RightButton)
  {
    ShowCursor(FALSE);
    m_controllingCamera = true;
  }
  else if (event->button() == Qt::MouseButton::LeftButton)
  {
    m_currentGizmoPart = GetGizmoPart(event->x(), event->y());
    if (m_currentGizmoPart != -1)
    {
      m_usingGizmo = true;
      m_lastIntersection = m_selection->GetComponent<Transform>()->GetWorldTranslation();
      m_lastMousePos = event->pos();
    }
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
    if (!m_usingGizmo)
      SelectObjectAt(event->x(), event->y());
    else
      m_usingGizmo = false;
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

  QPoint pos = event->pos();
  QPoint d = pos - m_lastMousePos;
  m_lastMousePos = pos;

  if (m_usingGizmo)
    UpdateGizmos(pos.x(), pos.y(), d.x(), d.y());
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

  m_box.Init(GraphicsManager::Instance()->GetDevice());
  m_box.CreateBox(0.1f, 0.1f, 1.0f);

  m_plane.Init(GraphicsManager::Instance()->GetDevice());
  m_plane.CreateBox(2.0f, 2.0f, 0.1f);
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

  GraphicsUtils::SetupCamera(context, (Real)width(), (Real)height(), SceneManager::Instance()->GetScene(), m_perFrameData);

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

      UINT32 handle = (UINT32)meshRenderer->GetOwner() + 1 + GIZMO_PART_COUNT;
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

  if (handle >= GIZMO_PART_COUNT + 1)
    emit ObjectSelected(SF_GET_OBJECT((HNDL)handle - 1 - GIZMO_PART_COUNT));
  else
    emit ObjectSelected(nullptr);

  context->Unmap(m_stagingTexture, 0);
}

GameWidget::GizmoPart GameWidget::GetGizmoPart(int x, int y)
{
  if (x < 0 || x >= width() || y < 0 || y >= height()) return (GizmoPart)-1;
  int px = (int)((Real)x / width() * 1024);
  int py = (int)((Real)y / height() * 1024);

  ID3D11DeviceContext *context = GraphicsManager::Instance()->GetDevice().GetImmediateContext().GetD3DResource();
  context->CopyResource(m_stagingTexture, m_pickingTarget.GetTexture()->GetD3DResource());

  D3D11_MAPPED_SUBRESOURCE msr;
  context->Map(m_stagingTexture, 0, D3D11_MAP_READ, 0, &msr);

  UINT32 *pickingData = (UINT32*)msr.pData;
  UINT32 handle = pickingData[py * 1024 + px];

  GizmoPart part = (GizmoPart)-1;
  if (handle >= 1 && handle <= GIZMO_PART_COUNT)
    part = (GizmoPart)(handle - 1);

  context->Unmap(m_stagingTexture, 0);
  return part;
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

    if (m_currentGizmo == TRANSLATION_GIZMO) RenderTranslationGizmo();
    if (m_currentGizmo == ROTATION_GIZMO) RenderRotationGizmo();
    if (m_currentGizmo == SCALING_GIZMO) RenderScalingGizmo();
  }
}

void GameWidget::RenderTranslationGizmo()
{
  QPoint mouse = this->mapFromGlobal(QCursor::pos());
  Transform *transform = m_selection->GetComponent<Transform>();

  D3D11Context& context = GraphicsManager::Instance()->GetDevice().GetImmediateContext();

  GraphicsUtils::SetupCamera(context, (Real)width(), (Real)height(), SceneManager::Instance()->GetScene(), m_perFrameData);

  m_pickingDepthBuffer.Clear(context);
  m_pickingTarget.Set(context, m_pickingDepthBuffer);

  DepthState::Set(context, DepthState::DEPTH_ENABLED);
  BlendState::Set(context, BlendState::NO_BLENDING);
  RasterState::Set(context, RasterState::BACK_FACE_CULLING);

  m_pickingVertexShader.Set(context);
  m_pickingPixelShader.Set(context);

  // Z axis
  PerObjectData perObject;
  perObject.WorldMatrix.SetTransformation(transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldForward());
  m_perObjectData->SetData(context, perObject);
  UINT32 handle = (UINT32)TRANSLATION_Z + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  // X axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldUp(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldRight());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)TRANSLATION_X + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  // Y axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldRight(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldUp());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)TRANSLATION_Y + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  GizmoPart part = GetGizmoPart(mouse.x(), mouse.y());
  DebugDraw::Instance()->DrawVector(transform->GetWorldTranslation(), transform->GetWorldRight(), true,   part == TRANSLATION_X ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(transform->GetWorldTranslation(), transform->GetWorldUp(), true,      part == TRANSLATION_Y ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawVector(transform->GetWorldTranslation(), transform->GetWorldForward(), true, part == TRANSLATION_Z ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

void GameWidget::RenderRotationGizmo()
{
  QPoint mouse = this->mapFromGlobal(QCursor::pos());
  Transform *transform = m_selection->GetComponent<Transform>();

  D3D11Context& context = GraphicsManager::Instance()->GetDevice().GetImmediateContext();

  GraphicsUtils::SetupCamera(context, (Real)width(), (Real)height(), SceneManager::Instance()->GetScene(), m_perFrameData);

  m_pickingDepthBuffer.Clear(context);
  m_pickingTarget.Set(context, m_pickingDepthBuffer);

  DepthState::Set(context, DepthState::DEPTH_ENABLED);
  BlendState::Set(context, BlendState::NO_BLENDING);
  RasterState::Set(context, RasterState::BACK_FACE_CULLING);

  m_pickingVertexShader.Set(context);
  m_pickingPixelShader.Set(context);

  // Z axis
  PerObjectData perObject;
  perObject.WorldMatrix.SetTransformation(transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation());
  m_perObjectData->SetData(context, perObject);
  UINT32 handle = (UINT32)ROTATION_Z + 1;
  m_pickingData->SetData(context, handle);
  m_plane.Draw(context);

  // X axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldUp(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)ROTATION_X + 1;
  m_pickingData->SetData(context, handle);
  m_plane.Draw(context);

  // Y axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldRight(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)ROTATION_Y + 1;
  m_pickingData->SetData(context, handle);
  m_plane.Draw(context);

  GizmoPart part = GetGizmoPart(mouse.x(), mouse.y());
  DebugDraw::Instance()->DrawCircle(transform->GetWorldTranslation(), transform->GetWorldRight(), 1.0f, true, part == ROTATION_X ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawCircle(transform->GetWorldTranslation(), transform->GetWorldUp(), 1.0f, true, part == ROTATION_Y ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawCircle(transform->GetWorldTranslation(), transform->GetWorldForward(), 1.0f, true, part == ROTATION_Z ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

void GameWidget::RenderScalingGizmo()
{
  QPoint mouse = this->mapFromGlobal(QCursor::pos());
  Transform *transform = m_selection->GetComponent<Transform>();

  D3D11Context& context = GraphicsManager::Instance()->GetDevice().GetImmediateContext();

  GraphicsUtils::SetupCamera(context, (Real)width(), (Real)height(), SceneManager::Instance()->GetScene(), m_perFrameData);

  m_pickingDepthBuffer.Clear(context);
  m_pickingTarget.Set(context, m_pickingDepthBuffer);

  DepthState::Set(context, DepthState::DEPTH_ENABLED);
  BlendState::Set(context, BlendState::NO_BLENDING);
  RasterState::Set(context, RasterState::BACK_FACE_CULLING);

  m_pickingVertexShader.Set(context);
  m_pickingPixelShader.Set(context);

  // Z axis
  PerObjectData perObject;
  perObject.WorldMatrix.SetTransformation(transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldForward());
  m_perObjectData->SetData(context, perObject);
  UINT32 handle = (UINT32)SCALING_Z + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  // X axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldUp(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldRight());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)SCALING_X + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  // Y axis
  perObject.WorldMatrix.SetTransformation(Quaternion(transform->GetWorldRight(), SF_PI / 2.0f) * transform->GetWorldRotation(), Vector3(1.0f, 1.0f, 1.0f), transform->GetWorldTranslation() + 0.5f * transform->GetWorldUp());
  m_perObjectData->SetData(context, perObject);
  handle = (UINT32)SCALING_Y + 1;
  m_pickingData->SetData(context, handle);
  m_box.Draw(context);

  GizmoPart part = GetGizmoPart(mouse.x(), mouse.y());
  DebugDraw::Instance()->DrawLine(transform->GetWorldTranslation(), transform->GetWorldTranslation() + transform->GetWorldRight(), true, part == SCALING_X ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawSphere(transform->GetWorldTranslation() + transform->GetWorldRight(), 0.02f, true, part == SCALING_X ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(1.0f, 0.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawLine(transform->GetWorldTranslation(), transform->GetWorldTranslation() + transform->GetWorldUp(), true, part == SCALING_Y ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawSphere(transform->GetWorldTranslation() + transform->GetWorldUp(), 0.02f, true, part == SCALING_Y ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 1.0f, 0.0f, 1.0f));
  DebugDraw::Instance()->DrawLine(transform->GetWorldTranslation(), transform->GetWorldTranslation() + transform->GetWorldForward(), true, part == SCALING_Z ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f));
  DebugDraw::Instance()->DrawSphere(transform->GetWorldTranslation() + transform->GetWorldForward(), 0.02f, true, part == SCALING_Z ? Vector4(1.0f, 1.0f, 0.0f, 1.0f) : Vector4(0.0f, 0.0f, 1.0f, 1.0f));
}

void GameWidget::UpdateGizmos(int x, int y, int dx, int dy)
{
  Transform *transform = m_selection->GetComponent<Transform>();
  Object *editorCamera = SF_GET_OBJECT(m_editorCamera);
  Transform *cameraTransform = editorCamera->GetComponent<Transform>();
  Camera *camera = editorCamera->GetComponent<Camera>();

  if (m_currentGizmo == TRANSLATION_GIZMO || m_currentGizmo == SCALING_GIZMO)
  {
    Real aspect = (Real)width() / height();
    Real fovy = camera->GetFieldOfView() / 2.0f;
    Real np = camera->GetNearPlane();
    Real npHeight = tanf(fovy) * np;
    Real npWidth = npHeight * aspect;
    Real npX = ((2.0f * x) / width() - 1.0f) * npWidth;
    Real npY = -((2.0f * y) / height() - 1.0f) * npHeight;

    Vector3 cameraRayPos = cameraTransform->GetWorldTranslation();
    Vector3 cameraRayDir = cameraTransform->GetWorldForward() * np + cameraTransform->GetWorldRight() * npX + cameraTransform->GetWorldUp() * npY;

    Vector3 oldCenter = transform->GetWorldTranslation();
    Vector3 intersection;
    switch (m_currentGizmoPart)
    {

    case TRANSLATION_X:
    case TRANSLATION_Y:
    case SCALING_X:
    case SCALING_Y:
    {
      Geometry::Plane plane(transform->GetWorldForward(), transform->GetWorldTranslation());

      Real t = 0.0f;
      Geometry::RayPlane(cameraRayPos, cameraRayDir, plane.m_data, t);
      intersection = cameraRayPos + t * cameraRayDir;
    } break;

    case TRANSLATION_Z:
    case SCALING_Z:
    {
      Geometry::Plane plane(transform->GetWorldUp(), transform->GetWorldTranslation());

      Real t = 0.0f;
      Geometry::RayPlane(cameraRayPos, cameraRayDir, plane.m_data, t);
      intersection = cameraRayPos + t * cameraRayDir;
    } break;

    }

    // Project onto axis
    if (m_currentGizmoPart == TRANSLATION_X || m_currentGizmoPart == SCALING_X) intersection = (intersection - m_lastIntersection).Dot(transform->GetWorldRight()) * transform->GetWorldRight() + m_lastIntersection;
    else if (m_currentGizmoPart == TRANSLATION_Y || m_currentGizmoPart == SCALING_Y) intersection = (intersection - m_lastIntersection).Dot(transform->GetWorldUp()) * transform->GetWorldUp() + m_lastIntersection;
    else if (m_currentGizmoPart == TRANSLATION_Z || m_currentGizmoPart == SCALING_Z) intersection = (intersection - m_lastIntersection).Dot(transform->GetWorldForward()) * transform->GetWorldForward() + m_lastIntersection;

    if (m_lastIntersection == transform->GetWorldTranslation())
    {
      m_lastIntersection = intersection;
      return;
    }

    if (m_currentGizmo == TRANSLATION_GIZMO)
    {
      Vector3 translation = intersection - m_lastIntersection;
      transform->SetTranslation(transform->GetWorldTranslation() + intersection - m_lastIntersection);
    }
    else
    {
      Matrix4 invRotation = transform->GetRotation().Inverted().GetMatrix4();
      Vector3 scaling = invRotation * (intersection - m_lastIntersection);
      transform->SetScale(transform->GetScale() + scaling);
    }

    m_lastIntersection = intersection;
  }
  else if (m_currentGizmo == ROTATION_GIZMO)
  {
    Vector3 rotationAxis;
    if (m_currentGizmoPart == ROTATION_X) rotationAxis = transform->GetRotation().GetMatrix4().TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
    else if (m_currentGizmoPart == ROTATION_Y) rotationAxis = transform->GetRotation().GetMatrix4().TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
    else if (m_currentGizmoPart == ROTATION_Z) rotationAxis = transform->GetRotation().GetMatrix4().TransformNormal(Vector3(0.0f, 0.0f, 1.0f));

    transform->SetRotation(Quaternion(rotationAxis, dx * 0.0001f * (cameraTransform->GetWorldTranslation() - transform->GetWorldTranslation()).Length()) * transform->GetRotation());
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

void GameWidget::BackupScene()
{
  std::ofstream file("temp_scene", std::ios_base::binary);
  Serialization::Serialize(file, SceneManager::Instance()->GetScene());
  file.close();
}

void GameWidget::RestoreScene()
{
  m_selection = nullptr;
  emit ObjectSelected(nullptr);

  std::ifstream file("temp_scene", std::ios_base::binary);
  Serialization::Deserialize(file, SceneManager::Instance()->GetScene());
  file.close();
  std::remove("temp_scene");
}

QPaintEngine* GameWidget::paintEngine() const
{
  return nullptr;
}

void GameWidget::SetSelection(Object *object)
{
  m_selection = object;
}

void GameWidget::MoveToObject(Object *object)
{
  Transform *cameraTransform = SF_GET_OBJECT(m_editorCamera)->GetComponent<Transform>();
  Transform *objectTransform = object->GetComponent<Transform>();
  cameraTransform->Update();
  objectTransform->Update();

  cameraTransform->SetTranslation(objectTransform->GetTranslation() - cameraTransform->GetForward() * 5.0f);
}

void GameWidget::SetGizmo(Gizmo gizmo)
{
  m_currentGizmo = gizmo;
}

void GameWidget::SetState(State state)
{
  if (state != m_state)
  {
    // Don't allow stopped to paused
    if (state == PAUSED && m_state == STOPPED)
      return;

    Time::Instance()->SetPaused(state != PLAYING);

    if (state == PLAYING)
    {
      if (m_state == STOPPED)
        BackupScene();
      SceneManager::Instance()->GetScene().SetCameraToFirst();
    }
    else if (m_state == PLAYING)
    {
      SceneManager::Instance()->GetScene().SetCameraObject(m_editorCamera);
    }

    if (state == STOPPED)
      RestoreScene();

    m_state = state;
  }
}

GameWidget::State GameWidget::GetState() const
{
  return m_state;
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
