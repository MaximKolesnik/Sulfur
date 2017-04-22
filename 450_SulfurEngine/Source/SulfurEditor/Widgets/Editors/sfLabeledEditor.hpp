/******************************************************************************/
/*!
\par     Sulfur
\file    sfLabeledEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Property editor with horizontal layout and text label

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfPropertyEditor.hpp"

namespace Sulfur
{
	
  class LabeledEditor : public PropertyEditor
  {
    Q_OBJECT

  public:
    LabeledEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    LabeledEditor(void *ptr, QWidget *parent = 0);
    ~LabeledEditor();

  private:
    void CreateLayout();
    void CreateLabel();

  protected:
    QBoxLayout *m_layout;
    QLabel *m_label;
  
  };
  
}