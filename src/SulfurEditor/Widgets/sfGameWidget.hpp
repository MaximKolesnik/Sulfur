/******************************************************************************/
/*!
\par     Sulfur
\file    sfGameWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Game rendering widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Window/sfWindow.hpp"
#include "Types/sfObject.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11VertexShader.hpp"
#include "Modules/Graphics/Resources/Shader/sfD3D11PixelShader.hpp"
#include "Modules/Graphics/Resources/Buffer/sfD3D11ConstantBuffer.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"

#include "Modules/Graphics/Target/sfDepthBuffer.hpp"
#include "Modules/Graphics/Target/sfRenderTarget.hpp"

namespace Sulfur
{
	
  class GameWidget : public QWidget
  {

    Q_OBJECT
    Q_DISABLE_COPY(GameWidget)

  public:
    enum Gizmo
    {
      TRANSLATION_GIZMO,
      ROTATION_GIZMO,
      SCALING_GIZMO
    };

    enum GizmoPart
    {
      TRANSLATION_X,
      TRANSLATION_Y,
      TRANSLATION_Z,
      ROTATION_X,
      ROTATION_Y,
      ROTATION_Z,
      SCALING_X,
      SCALING_Y,
      SCALING_Z,
      GIZMO_PART_COUNT
    };

  public:
    GameWidget(QWidget *parent = 0);
    ~GameWidget();

    void Frame();

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual QPaintEngine* paintEngine() const;

    void SetSelection(Object *object);
    void MoveToObject(Object *object);

    void SetGizmo(Gizmo widget);

  protected:
    virtual void resizeEvent(QResizeEvent* evt) override;
    virtual void paintEvent(QPaintEvent* evt) override;
    virtual void timerEvent(QTimerEvent* evt) override;

  private:
    void CreatePickingResources();
    void RenderPickingTexture();
    void SelectObjectAt(int x, int y);
    GizmoPart GetGizmoPart(int x, int y);
    void SelectionDrawing();
    void UpdateEditorCamera();

    // Gizmos
    void RenderTranslationGizmo();
    void RenderRotationGizmo();
    void RenderScalingGizmo();
    void UpdateGizmos(int x, int y, int dx, int dy);

  signals:
    void ObjectSelected(Object *object);

  private:
    Window *m_window;

    bool m_usingGizmo;
    GizmoPart m_currentGizmoPart;
    Vector3 m_lastIntersection;

    bool m_controllingCamera;
    HNDL m_editorCamera;
    Real m_cameraYaw;
    Real m_cameraPitch;

    int m_resizeTimer;
  
    Object *m_selection;
    Gizmo m_currentGizmo;
    QPoint m_lastMousePos;

    D3D11VertexShader m_pickingVertexShader;
    D3D11ConstantBuffer *m_perFrameData;
    D3D11ConstantBuffer *m_perObjectData;

    D3D11PixelShader m_pickingPixelShader;
    D3D11ConstantBuffer *m_pickingData;

    ID3D11Texture2D *m_stagingTexture;
    RenderTarget m_pickingTarget;
    DepthBuffer m_pickingDepthBuffer;

    Mesh m_box;
    Mesh m_plane;
  };
  
}