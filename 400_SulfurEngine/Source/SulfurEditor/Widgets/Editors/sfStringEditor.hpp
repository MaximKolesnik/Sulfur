/******************************************************************************/
/*!
\par     Sulfur
\file    sfStringEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   String property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class StringEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    StringEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    StringEditor(void *ptr, QWidget *parent = 0);
    ~StringEditor();

    virtual void UpdateValue() override;

  private:
    void CreateEdit();

  public slots:
    void OnEditingFinished();
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edit;
  
  };
  
}