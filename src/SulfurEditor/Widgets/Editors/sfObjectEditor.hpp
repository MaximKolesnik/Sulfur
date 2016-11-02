/******************************************************************************/
/*!
\par     Sulfur
\file    sfObjectEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Editor for objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfContainerEditor.hpp"
#include "Types/sfObject.hpp"

namespace Sulfur
{
	
  class ObjectEditor : public ContainerEditor
  {
    Q_OBJECT

  public:
    ObjectEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ObjectEditor(void *ptr, QWidget *parent = 0);
    ~ObjectEditor();

    virtual void UpdateValue() override;

  public slots:
    void OnAddComponentClicked();
    void OnAddComponent(QAction *action);
    void OnAddScript(QAction *action);

  private:
    QToolButton *m_newComponentButton;

  };
  
}