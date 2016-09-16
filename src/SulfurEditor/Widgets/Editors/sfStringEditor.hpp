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
#include "sfPropertyEditor.hpp"

namespace Sulfur
{
	
  class StringEditor : public PropertyEditor
  {
    Q_OBJECT

  public:
    StringEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ~StringEditor();

    virtual void UpdateValue() override;

  public slots:
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edit;
  
  };
  
}