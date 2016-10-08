/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector4Editor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector4 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class Vector4Editor : public LabeledEditor
  {
    Q_OBJECT

  public:
    Vector4Editor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    Vector4Editor(void *ptr, QWidget *parent = 0);
    ~Vector4Editor();

    virtual void UpdateValue() override;

  private:
    void CreateEdits();

  public slots:
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edits[4];
  
  };
  
}