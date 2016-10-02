/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/28/2016

\brief   Resource property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"
#include "sfResourceBox.hpp"

namespace Sulfur
{
	
  class ResourceEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    ResourceEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ResourceEditor(void *ptr, QWidget *parent = 0);
    ~ResourceEditor();

    virtual void UpdateValue() override;

  private:
    void CreateEdit();

  public slots:
    void OnValueChanged(const QString& value);

  private:
    ResourceBox *m_edit;
  
  };
  
}