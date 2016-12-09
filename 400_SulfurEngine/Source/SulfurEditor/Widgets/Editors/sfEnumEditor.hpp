/******************************************************************************/
/*!
\par     Sulfur
\file    sfEnumEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/19/2016

\brief   Enum property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class EnumEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    EnumEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    EnumEditor(void *ptr, QWidget *parent = 0);
    ~EnumEditor();

    virtual void UpdateValue() override;

  private:
    void CreateComboBox();

  public slots:
    void OnValueSelected(int index);

  private:
    QComboBox *m_comboBox;
  
  };
  
}