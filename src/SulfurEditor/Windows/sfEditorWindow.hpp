/******************************************************************************/
/*!
\par     Sulfur
\file    sfEditorWindow.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Main editor window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <QtWidgets/QMainWindow>
#include "Widgets/sfEditorWidget.hpp"
#include "Widgets/sfInspectorWidget.hpp"
#include "Widgets/sfResourceBrowserWidget.hpp"
#include "Widgets/sfOutputWidget.hpp"
#include "Widgets/sfSceneBrowserWidget.hpp"
#include "sfAboutWindow.hpp"

namespace Sulfur
{

  class EditorWindow : public QMainWindow
  {

    Q_OBJECT

  public:
    EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

    void Frame();

  private:
    void CreateMenuBar();
    void OnOpenScene();
    void OnSaveScene();
    void OnSaveSceneAs();
    void OnDebugDraw();
    void OnAbout();

  public slots:
    void OnObjectActivated(Object *object);
    void OnObjectSelected(Object *object);
    void OnObjectSelectedGameWindow(Object *object);
    void OnObjectChanged();

  private:
    QMenuBar *m_menuBar;
    QAction *m_debugDrawAction;

    QToolBar *m_toolBar;

    EditorWidget *m_editor;
    InspectorWidget *m_inspector;
    ResourceBrowserWidget *m_resourceBrowser;
    OutputWidget *m_output;
    SceneBrowserWidget *m_sceneBrowser;

    AboutWindow *m_aboutWindow;

  };

}
