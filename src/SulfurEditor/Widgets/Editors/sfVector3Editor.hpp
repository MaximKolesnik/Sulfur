/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector3Editor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector3 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class Vector3Editor : public LabeledEditor
  {
    Q_OBJECT

  public:
    Vector3Editor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    Vector3Editor(void *ptr, QWidget *parent = 0);
    ~Vector3Editor();

    virtual void UpdateValue() override;

  private:
    void CreateEdits();

  public slots:
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edits[3];
  
  };
  
}