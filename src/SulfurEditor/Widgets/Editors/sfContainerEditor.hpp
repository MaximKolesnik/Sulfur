/******************************************************************************/
/*!
\par     Sulfur
\file    sfContainerEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Base property editor for container types (vectors, lists, etc)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfPropertyEditor.hpp"

namespace Sulfur
{
	
  class ContainerEditor : public PropertyEditor
  {
    Q_OBJECT

  public:
    ContainerEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ContainerEditor(void *ptr, QWidget *parent = 0);
    ~ContainerEditor();

    void AddChild(PropertyEditor *editor);

    void ClearLayout();

  public slots:
    void OnChildChanged();

  private:
    void Setup();
    QBoxLayout* CreateLayout();

  protected:
    QBoxLayout *m_mainLayout;

    QWidget *m_childrenWidget;
    QBoxLayout *m_childrenLayout;
  
  };
  
}