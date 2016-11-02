/******************************************************************************/
/*!
\par     Sulfur
\file    sfCollapsableEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/19/2016

\brief   Collapsable container property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfContainerEditor.hpp"

namespace Sulfur
{
	
  class CollapsableEditor : public ContainerEditor
  {
    Q_OBJECT

  public:
    CollapsableEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    CollapsableEditor(void *ptr, QWidget *parent = 0);
    ~CollapsableEditor();

    void SetHeaderText(const std::string& text);

  private:
    void Setup();

  public slots:
    void OnCollapseClicked(bool checked);

  protected:
    QPushButton *m_collapseButton;
  
  };
  
}