/******************************************************************************/
/*!
\par     Sulfur
\file    sfVector2Editor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Vector2 property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class Vector2Editor : public LabeledEditor
  {
    Q_OBJECT

  public:
    Vector2Editor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    Vector2Editor(void *ptr, QWidget *parent = 0);
    ~Vector2Editor();

    virtual void UpdateValue() override;

  private:
    void CreateEdits();

  public slots:
    void OnEditingFinished();
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edits[2];
  
  };
  
}