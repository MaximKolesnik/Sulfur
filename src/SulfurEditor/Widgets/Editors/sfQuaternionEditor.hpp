/******************************************************************************/
/*!
\par     Sulfur
\file    sfQuaternionEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Quaternion property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class QuaternionEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    QuaternionEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    QuaternionEditor(void *ptr, QWidget *parent = 0);
    ~QuaternionEditor();

    virtual void UpdateValue() override;

  private:
    void CreateEdits();

  public slots:
    void OnEditingFinished();
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edits[3];
  
  };
  
}