/******************************************************************************/
/*!
\par     Sulfur
\file    sfBoolEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/5/2016

\brief   Boolean property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class BoolEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    BoolEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    BoolEditor(void *ptr, QWidget *parent = 0);
    ~BoolEditor();

    virtual void UpdateValue() override;

  private:
    void CreateCheckBox();

  public slots:
    void OnStateChanged(int state);

  private:
    QCheckBox *m_checkBox;
  
  };
  
}